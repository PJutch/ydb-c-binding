#include "ydb-c-sdk/datetime.h"

#include <__ydb_sdk_special_headers/util/datetime/base.h>

YdbInstant ParseIso8601(char* date) {
    return static_cast<void*>(new TInstant{TInstant::ParseIso8601(date)});
}
