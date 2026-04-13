#include "ydb-c-sdk/datetime.h"

#include "helpers.hpp"

#include <__ydb_sdk_special_headers/util/datetime/base.h>

#include <string.h>

YdbInstant YdbInstantParseIso8601(char* date) {
    return {static_cast<void*>(new TInstant{TInstant::ParseIso8601(date)})};
}

char* YdbFormatLocalTime(YdbInstant date, char* format_string) {
    return strdup(FROM_OPAQUE(TInstant, date).FormatLocalTime(format_string).data());
}
