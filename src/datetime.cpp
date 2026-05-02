#include "ydb-c-sdk/datetime.h"

#include "helpers.hpp"

#include <__ydb_sdk_special_headers/util/datetime/base.h>

#include <string.h>

YdbInstant YdbInstantZero() {
    return TO_NEW_OPAQUE(TInstant, TInstant::Zero());
}

YdbInstant YdbInstantMax() {
    return TO_NEW_OPAQUE(TInstant, TInstant::Max());
}

YdbInstant YdbInstantNow() {
    return TO_NEW_OPAQUE(TInstant, TInstant::Now());
}

#define YDB_INSTANT_FROM(units) \
    YdbInstant YdbInstantFrom##units(uint64_t value) { \
        return TO_NEW_OPAQUE(TInstant, TInstant::units(value)); \
    }

YDB_INSTANT_FROM(Days)
YDB_INSTANT_FROM(Hours)
YDB_INSTANT_FROM(MicroSeconds)
YDB_INSTANT_FROM(MilliSeconds)
YDB_INSTANT_FROM(Minutes)
YDB_INSTANT_FROM(Seconds)

#define YDB_INSTANT_TO(units) \
    uint64_t YdbInstantTo##units(YdbInstant instant) { \
        return FROM_OPAQUE(TInstant, instant).units(); \
    }

YDB_INSTANT_TO(Days)
YDB_INSTANT_TO(Hours)
YDB_INSTANT_TO(MicroSeconds)
YDB_INSTANT_TO(MilliSeconds)
YDB_INSTANT_TO(Minutes)
YDB_INSTANT_TO(Seconds)

char* YdbInstantToIso8601(YdbInstant instant) {
    return strdup(FROM_OPAQUE(TInstant, instant).ToString().data());
}

char* YdbInstantToIso8601UpToSeconds(YdbInstant instant) {
    return strdup(FROM_OPAQUE(TInstant, instant).ToStringUpToSeconds().data());
}

char* YdbInstantToIso8601Local(YdbInstant instant) {
    return strdup(FROM_OPAQUE(TInstant, instant).ToStringLocal().data());
}

char* YdbInstantToIso8601LocalUpToSeconds(YdbInstant instant) {
    return strdup(FROM_OPAQUE(TInstant, instant).ToStringLocalUpToSeconds().data());
}

char* YdbInstantToRfc822(YdbInstant instant) {
    return strdup(FROM_OPAQUE(TInstant, instant).ToRfc822String().data());
}
char* YdbInstantToRfc822Local(YdbInstant instant) {
    return strdup(FROM_OPAQUE(TInstant, instant).ToRfc822StringLocal().data());
}

char* YdbFormatLocalTime(YdbInstant date, char* format_string) {
    return strdup(FROM_OPAQUE(TInstant, date).FormatLocalTime(format_string).data());
}

char* YdbFormatGmTime(YdbInstant date, char* format_string) {
    return strdup(FROM_OPAQUE(TInstant, date).FormatGmTime(format_string).data());
}

#define YDB_INSTANT_PARSE(format) \
    YdbInstant YdbInstantParse##format(char* instant, bool* ok) { \
        TInstant parsed; \
        bool parse_success = TInstant::TryParse##format(instant, parsed); \
        \
        if (ok) { \
            *ok = parse_success; \
        } \
        \
        return TO_NEW_OPAQUE(TInstant, parsed); \
    }

YDB_INSTANT_PARSE(Iso8601)
YDB_INSTANT_PARSE(Rfc822)
YDB_INSTANT_PARSE(Http)
YDB_INSTANT_PARSE(X509)

YdbDuration YdbDurationZero() {
    return TO_NEW_OPAQUE(TDuration, TDuration::Zero());
}

YdbDuration YdbDurationMax() {
    return TO_NEW_OPAQUE(TDuration, TDuration::Max());
}

#define YDB_DURATION_FROM(units) \
    YdbDuration YdbDurationFrom##units(uint64_t value) { \
        return TO_NEW_OPAQUE(TDuration, TDuration::units(value)); \
    }

YDB_DURATION_FROM(Days)
YDB_DURATION_FROM(Hours)
YDB_DURATION_FROM(MicroSeconds)
YDB_DURATION_FROM(MilliSeconds)
YDB_DURATION_FROM(Minutes)
YDB_DURATION_FROM(Seconds)

#define YDB_DURATION_TO(units) \
    uint64_t YdbDurationTo##units(YdbDuration instant) { \
        return FROM_OPAQUE(TDuration, instant).units(); \
    }

YDB_DURATION_TO(Days)
YDB_DURATION_TO(Hours)
YDB_DURATION_TO(MicroSeconds)
YDB_DURATION_TO(MilliSeconds)
YDB_DURATION_TO(Minutes)
YDB_DURATION_TO(Seconds)

char* YdbDurationToString(YdbDuration duration) {
    return strdup(FROM_OPAQUE(TDuration, duration).ToString().Data());
}

YdbDuration YdbDurationParse(char* duration, bool* ok) {
    TDuration parsed;
    bool parse_success = TDuration::TryParse(duration, parsed);

    if (ok) {
        *ok = parse_success;
    }

    return TO_NEW_OPAQUE(TDuration, parsed);
}

YdbDuration YdbInstantDifference(YdbInstant past, YdbInstant future) {
    return TO_NEW_OPAQUE(TDuration, FROM_OPAQUE(TDuration, future) - FROM_OPAQUE(TDuration, past));
}

YdbInstant YdbInstantAfter(YdbInstant base, YdbDuration duration) {
    return TO_NEW_OPAQUE(TInstant, FROM_OPAQUE(TInstant, base) + FROM_OPAQUE(TDuration, duration));
}

YdbInstant YdbInstantBefore(YdbInstant base, YdbDuration duration) {
    return TO_NEW_OPAQUE(TInstant, FROM_OPAQUE(TInstant, base) - FROM_OPAQUE(TDuration, duration));
}

YdbDuration YdbDurationSum(YdbDuration first, YdbDuration second) {
    return TO_NEW_OPAQUE(TDuration, FROM_OPAQUE(TDuration, first) + FROM_OPAQUE(TDuration, second));
}

YdbDuration YdbDurationDiff(YdbDuration first, YdbDuration second) {
    return TO_NEW_OPAQUE(TDuration, FROM_OPAQUE(TDuration, first) - FROM_OPAQUE(TDuration, second));
}
