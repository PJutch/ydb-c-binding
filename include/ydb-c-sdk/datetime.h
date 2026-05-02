#ifndef YDB_C_SDK_DATETIME_H_
#define YDB_C_SDK_DATETIME_H_

#include "helpers.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbInstant)

void YdbDestroyInstant(YdbInstant instant);

#define YDB_NULL_INSTANT (YdbInstant){NULL}

YdbInstant YdbInstantZero();
YdbInstant YdbInstantMax();

YdbInstant YdbInstantNow();

YdbInstant YdbInstantFromDays(uint64_t value);
YdbInstant YdbInstantFromHours(uint64_t value);
YdbInstant YdbInstantFromMicroSeconds(uint64_t value);
YdbInstant YdbInstantFromMilliSeconds(uint64_t value);
YdbInstant YdbInstantFromMinutes(uint64_t value);
YdbInstant YdbInstantFromSeconds(uint64_t value);

uint64_t YdbInstantToDays(YdbInstant instant);
uint64_t YdbInstantToHours(YdbInstant instant);
uint64_t YdbInstantToMicroSeconds(YdbInstant instant);
uint64_t YdbInstantToMilliSeconds(YdbInstant instant);
uint64_t YdbInstantToMinutes(YdbInstant instant);
uint64_t YdbInstantToSeconds(YdbInstant instant);

// Deallocate using free()
char* YdbInstantToIso8601(YdbInstant instant);
char* YdbInstantToIso8601UpToSeconds(YdbInstant instant);
char* YdbInstantToIso8601Local(YdbInstant instant);
char* YdbInstantToIso8601LocalUpToSeconds(YdbInstant instant);
char* YdbInstantToRfc822(YdbInstant instant);
char* YdbInstantToRfc822Local(YdbInstant instant);

// Deallocate using free()
char* YdbFormatLocalTime(YdbInstant instant, char* format_string);
char* YdbFormatGmTime(YdbInstant instant, char* format_string);

// ok can be null to ignore errors
YdbInstant YdbInstantParseIso8601(char* instant, bool* ok);
YdbInstant YdbInstantParseRfc822(char* instant, bool* ok);
YdbInstant YdbInstantParseHttp(char* instant, bool* ok);
YdbInstant YdbInstantParseX509(char* instant, bool* ok);

YDB_C_SDK_OPAQUE_STRUCT(YdbDuration)

void YdbDestroyDuration(YdbDuration duration);

#define YDB_NULL_DURATION (YdbDuration){NULL}

YdbDuration YdbDurationZero();
YdbDuration YdbDurationMax();

YdbDuration YdbDurationFromDays(uint64_t value);
YdbDuration YdbDurationFromHours(uint64_t value);
YdbDuration YdbDurationFromMicroSeconds(uint64_t value);
YdbDuration YdbDurationFromMilliSeconds(uint64_t value);
YdbDuration YdbDurationFromMinutes(uint64_t value);
YdbDuration YdbDurationFromSeconds(uint64_t value);

uint64_t YdbDurationToDays(YdbDuration duration);
uint64_t YdbDurationToHours(YdbDuration duration);
uint64_t YdbDurationToMicroSeconds(YdbDuration duration);
uint64_t YdbDurationToMilliSeconds(YdbDuration duration);
uint64_t YdbDurationToMinutes(YdbDuration duration);
uint64_t YdbDurationToSeconds(YdbDuration duration);

// Deallocate using free()
char* YdbDurationToString(YdbDuration duration);

YdbDuration YdbDurationParse(char* duration, bool* ok);

YdbDuration YdbInstantDifference(YdbInstant past, YdbInstant future);
YdbInstant YdbInstantAfter(YdbInstant base, YdbDuration duration);
YdbInstant YdbInstantBefore(YdbInstant base, YdbDuration duration);

YdbDuration YdbDurationSum(YdbDuration first, YdbDuration second);
YdbDuration YdbDurationDiff(YdbDuration first, YdbDuration second);


#ifdef __cplusplus
}
#endif

#endif