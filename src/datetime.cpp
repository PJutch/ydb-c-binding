#include "ydb-c-sdk/datetime.h"

#include "helpers.hpp"

#include <__ydb_sdk_special_headers/util/datetime/base.h>

#include <string.h>

extern "C" {

YdbInstant YdbInstantNow() { return TInstant::Now().GetValue(); }

#define YDB_INSTANT_FROM(units)                                                \
    YdbInstant YdbInstantFrom##units(uint64_t value) {                         \
        return TInstant::units(value).GetValue();                              \
    }

YDB_INSTANT_FROM(Days)
YDB_INSTANT_FROM(Hours)
YDB_INSTANT_FROM(MicroSeconds)
YDB_INSTANT_FROM(MilliSeconds)
YDB_INSTANT_FROM(Minutes)
YDB_INSTANT_FROM(Seconds)

#define YDB_INSTANT_TO(units)                                                  \
    uint64_t YdbInstantTo##units(YdbInstant instant) {                         \
        return TInstant::FromValue(instant).units();                           \
    }

YDB_INSTANT_TO(Days)
YDB_INSTANT_TO(Hours)
YDB_INSTANT_TO(MicroSeconds)
YDB_INSTANT_TO(MilliSeconds)
YDB_INSTANT_TO(Minutes)
YDB_INSTANT_TO(Seconds)

char* YdbInstantToIso8601(YdbInstant instant) {
    return strdup(TInstant::FromValue(instant).ToString().data());
}

char* YdbInstantToIso8601UpToSeconds(YdbInstant instant) {
    return strdup(TInstant::FromValue(instant).ToStringUpToSeconds().data());
}

char* YdbInstantToIso8601Local(YdbInstant instant) {
    return strdup(TInstant::FromValue(instant).ToStringLocal().data());
}

char* YdbInstantToIso8601LocalUpToSeconds(YdbInstant instant) {
    return strdup(
        TInstant::FromValue(instant).ToStringLocalUpToSeconds().data());
}

char* YdbInstantToRfc822(YdbInstant instant) {
    return strdup(TInstant::FromValue(instant).ToRfc822String().data());
}
char* YdbInstantToRfc822Local(YdbInstant instant) {
    return strdup(TInstant::FromValue(instant).ToRfc822StringLocal().data());
}

char* YdbFormatLocalTime(YdbInstant date, char* format_string) {
    return strdup(
        TInstant::FromValue(date).FormatLocalTime(format_string).data());
}

char* YdbFormatGmTime(YdbInstant date, char* format_string) {
    return strdup(TInstant::FromValue(date).FormatGmTime(format_string).data());
}

#define YDB_INSTANT_PARSE(format)                                              \
    YdbInstant YdbInstantParse##format(char* instant, bool* ok) {              \
        TInstant parsed;                                                       \
        bool parse_success = TInstant::TryParse##format(instant, parsed);      \
                                                                               \
        if (ok) {                                                              \
            *ok = parse_success;                                               \
        }                                                                      \
                                                                               \
        return parsed.GetValue();                                              \
    }

YDB_INSTANT_PARSE(Iso8601)
YDB_INSTANT_PARSE(Rfc822)
YDB_INSTANT_PARSE(Http)
YDB_INSTANT_PARSE(X509)

#define YDB_DURATION_FROM(units)                                               \
    YdbDuration YdbDurationFrom##units(uint64_t value) {                       \
        return TDuration::units(value).GetValue();                             \
    }

YDB_DURATION_FROM(Days)
YDB_DURATION_FROM(Hours)
YDB_DURATION_FROM(MicroSeconds)
YDB_DURATION_FROM(MilliSeconds)
YDB_DURATION_FROM(Minutes)
YDB_DURATION_FROM(Seconds)

#define YDB_DURATION_TO(units)                                                 \
    uint64_t YdbDurationTo##units(YdbDuration instant) {                       \
        return TDuration::FromValue(instant).units();                          \
    }

YDB_DURATION_TO(Days)
YDB_DURATION_TO(Hours)
YDB_DURATION_TO(MicroSeconds)
YDB_DURATION_TO(MilliSeconds)
YDB_DURATION_TO(Minutes)
YDB_DURATION_TO(Seconds)

char* YdbDurationToString(YdbDuration duration) {
    return strdup(TDuration::FromValue(duration).ToString().Data());
}

YdbDuration YdbDurationParse(char* duration, bool* ok) {
    TDuration parsed;
    bool parse_success = TDuration::TryParse(duration, parsed);

    if (ok) {
        *ok = parse_success;
    }

    return parsed.GetValue();
}
}
