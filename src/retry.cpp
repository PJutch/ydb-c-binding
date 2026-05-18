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
} YdbRetrierImpl;

YdbRetrier YdbCreateRetrier() {
    return TO_NEW_OPAQUE(YdbRetrierImpl, 0, 10, YdbInstantNow(),
                         YDB_DURATION_MAX);
}

YdbRetryNextStep YdbRetrierNext(YdbRetrier retrier_, YdbStatus status_) {
    YdbRetrierImpl& retrier = YdbFromOpaque<YdbRetrierImpl>(retrier_);
    NYdb::TStatus& status = YdbFromOpaque<NYdb::TStatus>(status_);

    if (status.IsSuccess()) {
        return YDB_RETRY_FINISH;
    }

    if (retrier.retry_number >= retrier.max_retries) {
        return YDB_RETRY_FINISH;
    }
    ++retrier.retry_number;

    if (YdbInstantNow() - retrier.start_time >= retrier.max_duration) {
        return YDB_RETRY_FINISH;
    }

    switch (status.GetStatus()) {
    case NYdb::EStatus::ABORTED:
        return YDB_RETRY_IMMEDIATELY;

    case NYdb::EStatus::OVERLOADED:
    case NYdb::EStatus::CLIENT_RESOURCE_EXHAUSTED:
        return YDB_RETRY_SLOWBACKOFF;

    case NYdb::EStatus::UNAVAILABLE:
        return YDB_RETRY_FASTBACKOFF;

    case NYdb::EStatus::BAD_SESSION:
    case NYdb::EStatus::SESSION_BUSY:
        return YDB_RETRY_IMMEDIATELY_RESET;

    case NYdb::EStatus::NOT_FOUND:
        if (retrier.retry_not_found) {
            return YDB_RETRY_IMMEDIATELY;
        } else {
            return YDB_RETRY_FINISH;
        }

    case NYdb::EStatus::UNDETERMINED:
        if (retrier.idempotent) {
            return YDB_RETRY_FASTBACKOFF;
        } else {
            return YDB_RETRY_FINISH;
        }

    case NYdb::EStatus::TRANSPORT_UNAVAILABLE:
        if (retrier.idempotent) {
            return YDB_RETRY_FASTBACKOFF_RESET;
        } else {
            return YDB_RETRY_FINISH;
        }

    default:
        return retrier.retry_undefined ? YDB_RETRY_SLOWBACKOFF
                                       : YDB_RETRY_FINISH;
    }
}

uint32_t YdbGetRetryNumber(YdbRetrier retrier) {
    return YdbFromOpaque<YdbRetrierImpl>(retrier).retry_number;
}

void YdbDestroyRetrier(YdbRetrier retrier) {
    delete YdbPtrFromOpaque<YdbRetrierImpl>(retrier);
}
}
