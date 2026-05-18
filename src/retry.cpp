#include "ydb-c-sdk/retry.h"
#include "helpers.hpp"
#include "ydb-c-sdk/datetime.h"

#include <ydb-cpp-sdk/client/query/query.h>

extern "C" {

typedef struct YdbRetrierImpl {
    uint32_t retry_number;
    uint32_t max_retries;

    YdbInstant start_time;
    YdbDuration max_duration;

    bool retry_not_found;
    bool idempotent;
    bool retry_undefined;

    uint32_t max_fast_backoff_slots;
    YdbDuration fast_backoff_slot_duration;
    double fast_backoff_uncertaincy_ratio;

    uint32_t max_slow_backoff_slots;
    YdbDuration slow_backoff_slot_duration;
    double slow_backoff_uncertaincy_ratio;
} YdbRetrierImpl;

YdbRetrier YdbCreateRetrier() {
    return TO_NEW_OPAQUE(YdbRetrierImpl, 0, 10, YdbInstantNow(),
                         YDB_DURATION_MAX);
}

typedef enum YdbRetryBackoffMode {
    YDB_BACKOFF_NONE,
    YDB_BACKOFF_NONE_RESET,
    YDB_BACKOFF_FAST,
    YDB_BACKOFF_FAST_RESET,
    YDB_BACKOFF_SLOW,
    YDB_BACKOFF_FINISH,
} YdbRetryBackoffMode;

static YdbRetryBackoffMode YdbRetrierGetBackoff(YdbRetrierImpl retrier,
                                                NYdb::TStatus& status) {
    if (status.IsSuccess()) {
        return YDB_BACKOFF_FINISH;
    }

    if (retrier.retry_number >= retrier.max_retries) {
        return YDB_BACKOFF_FINISH;
    }

    if (YdbInstantNow() - retrier.start_time >= retrier.max_duration) {
        return YDB_BACKOFF_FINISH;
    }

    switch (status.GetStatus()) {
    case NYdb::EStatus::ABORTED:
        return YDB_BACKOFF_NONE;

    case NYdb::EStatus::OVERLOADED:
    case NYdb::EStatus::CLIENT_RESOURCE_EXHAUSTED:
        return YDB_BACKOFF_SLOW;

    case NYdb::EStatus::UNAVAILABLE:
        return YDB_BACKOFF_FAST;

    case NYdb::EStatus::BAD_SESSION:
    case NYdb::EStatus::SESSION_BUSY:
        return YDB_BACKOFF_NONE_RESET;

    case NYdb::EStatus::NOT_FOUND:
        if (retrier.retry_not_found) {
            return YDB_BACKOFF_NONE;
        } else {
            return YDB_BACKOFF_FINISH;
        }

    case NYdb::EStatus::UNDETERMINED:
        if (retrier.idempotent) {
            return YDB_BACKOFF_FAST;
        } else {
            return YDB_BACKOFF_FINISH;
        }

    case NYdb::EStatus::TRANSPORT_UNAVAILABLE:
        if (retrier.idempotent) {
            return YDB_BACKOFF_FAST_RESET;
        } else {
            return YDB_BACKOFF_FINISH;
        }

    default:
        return retrier.retry_undefined ? YDB_BACKOFF_SLOW : YDB_BACKOFF_FINISH;
    }
}

static void DoBackoff(bool fast, YdbRetrierImpl& retrier) {
    uint32_t max_backoff_slots =
        fast ? retrier.max_fast_backoff_slots : retrier.max_slow_backoff_slots;
    YdbDuration backoff_slot_duration =
        fast ? retrier.fast_backoff_slot_duration
             : retrier.slow_backoff_slot_duration;
    double uncertainty_ratio = fast ? retrier.fast_backoff_uncertaincy_ratio
                                    : retrier.slow_backoff_uncertaincy_ratio;

    uint32_t backoff_slots =
        1 << std::min(retrier.retry_number, max_backoff_slots);
    if (backoff_slots > (1 << 6)) {
        backoff_slots = (1 << 6);
    }

    YdbDuration max_duration = backoff_slot_duration * backoff_slots;

    double uncertainty_multiplier =
        ((double)rand()) / RAND_MAX * uncertainty_ratio - uncertainty_ratio +
        1.0;

    usleep(max_duration * uncertainty_multiplier);
}

YdbRetryNextStep YdbRetrierNext(YdbRetrier retrier_, YdbStatus status_) {
    YdbRetrierImpl& retrier = YdbFromOpaque<YdbRetrierImpl>(retrier_);
    NYdb::TStatus& status = YdbFromOpaque<NYdb::TStatus>(status_);

    YdbRetryBackoffMode backoff_mode = YdbRetrierGetBackoff(retrier, status);
    ++retrier.max_retries;

    switch (backoff_mode) {
    case YDB_BACKOFF_FAST:
        DoBackoff(true, retrier);
        return YDB_RETRY_RETRY;
    case YDB_BACKOFF_FAST_RESET:
        DoBackoff(true, retrier);
        return YDB_RETRY_RESET;
    case YDB_BACKOFF_SLOW:
        DoBackoff(false, retrier);
        return YDB_RETRY_RETRY;
    case YDB_BACKOFF_NONE:
        return YDB_RETRY_RETRY;
    case YDB_BACKOFF_NONE_RESET:
        return YDB_RETRY_RESET;
    case YDB_BACKOFF_FINISH:
        return YDB_RETRY_FINISH;
    }
    assert(false);
}

uint32_t YdbGetRetryNumber(YdbRetrier retrier) {
    return YdbFromOpaque<YdbRetrierImpl>(retrier).retry_number;
}

void YdbDestroyRetrier(YdbRetrier retrier) {
    delete YdbPtrFromOpaque<YdbRetrierImpl>(retrier);
}
}
