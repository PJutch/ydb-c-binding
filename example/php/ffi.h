#define FFI_LIB "/home/pjutch/test-install/lib/libydb-c-sdk.so"

#include <stdint.h>

void free(void*);

typedef struct {
    void* data;
} YdbDriverConfig;

YdbDriverConfig YdbCreateDriverConfig();
YdbDriverConfig YdbCreateDriverConfigConnectionString(char* connectionString);

void YdbDestroyDriverConfig(YdbDriverConfig config);

void YdbDriverConfigSetEndpoint(YdbDriverConfig config, char* endpoint);
void YdbDriverConfigSetDatabase(YdbDriverConfig config, char* database);
void YdbDriverConfigUseSecureConnection(YdbDriverConfig config, char* caCert);

typedef struct {
    void* data;
} YdbDriver;

YdbDriver YdbCreateDriver(YdbDriverConfig config);
void YdbStopDriver(YdbDriver driver, bool wait);

typedef struct {
    void* data;
} YdbQueryClient;

YdbQueryClient YdbCreateQueryClient(YdbDriver driver);
void YdbDestroyQueryClient(YdbQueryClient client);

typedef uint64_t YdbInstant;

YdbInstant YdbInstantParseIso8601(char* instant, bool* ok);

char* YdbFormatLocalTime(YdbInstant instant, char* format_string);

typedef struct {
    void* data;
} YdbQueryResult;

void YdbDestroyResult(YdbQueryResult result);

typedef struct {
    void* data;
} YdbStatus;

YdbStatus YdbStatusOk();

void YdbDestroyStatus(YdbStatus status);

bool YdbIsSuccess(YdbStatus status);
bool YdbIsTransportError(YdbStatus status);

// Deallocate using free()
char* YdbGetErrorMessage(YdbStatus status);

// Doesn't copy, don't destroy both
YdbStatus YdbAsStatus(YdbQueryResult result);

typedef struct {
    void* data;
} YdbResultSet;

YdbResultSet YdbGetResultSet(YdbQueryResult result, int result_index);
void DestroyResultSet(YdbResultSet result_set);

typedef struct {
    void* data;
} YdbResultSetParser;

YdbResultSetParser YdbCreateResultSetParser(YdbResultSet result_set);
void YdbDestroyResultSetParser(YdbResultSetParser result_set_parser);

bool YdbNextRow(YdbResultSetParser result_set_parser);

typedef struct {
    void* data;
} YdbValueParser;

// References result_set_parser
YdbValueParser YdbColumnParser(YdbResultSetParser result_set_parser,
                               char* column_name);

// If exists is NULL treats it as required
uint64_t YdbParseUint64(YdbValueParser parser, bool* exists);

// May return NULL
char* YdbParseUtf8(YdbValueParser parser);

// ok may be NULL to ignore errors
YdbInstant YdbParseDate(YdbValueParser parser, bool* ok);

typedef struct {
    void* data;
} YdbParamsBuilder;

YdbParamsBuilder YdbCreateParamsBuilder();

typedef struct {
    void* data;
} YdbParamValueBuilder;

YdbParamValueBuilder YdbAddParam(YdbParamsBuilder builder, char* name);

void YdbParamBeginList(YdbParamValueBuilder builder);
void YdbParamAddListItem(YdbParamValueBuilder builder);
void YdbParamEndList(YdbParamValueBuilder builder);

void YdbParamBeginStruct(YdbParamValueBuilder builder);
void YdbParamAddMember(YdbParamValueBuilder builder, char* name);
void YdbParamEndStruct(YdbParamValueBuilder builder);

void YdbParamUtf8(YdbParamValueBuilder builder, char* value);
void YdbParamUint8(YdbParamValueBuilder builder, uint8_t value);
void YdbParamUint64(YdbParamValueBuilder builder, uint64_t value);
void YdbParamDate(YdbParamValueBuilder builder, YdbInstant date);

typedef struct {
    void* data;
} YdbParams;

#define YDB_NULL_PARAMS ((YdbParams){NULL})

void YdbBuildParamValue(YdbParamValueBuilder builder);
YdbParams YdbBuildParams(YdbParamsBuilder builder);

void YdbDestroyParams(YdbParams);

typedef struct {
    void* data;
} YdbSession;

typedef enum YdbTxMode {
    YDB_TX_SERIALIZABLE_RW,
    YDB_TX_ONLINE_RO,
    YDB_TX_STALE_RO,
    YDB_TX_SNAPSHOT_RO,
    YDB_TX_SNAPSHOT_RW,
    YDB_TX_TRANSACTION,
} YdbTxMode;

typedef struct {
    void* data;
} YdbTransaction;

#define NULL_TRANSACTION (YdbTransaction){NULL};

typedef struct YdbTx {
    YdbTxMode mode;
    bool commit;
    bool allow_inconsistent_reads;
    YdbTransaction transaction;
} YdbTx;

YdbQueryResult YdbExecuteQuerySync(YdbSession session, char* query, YdbTx* tx,
                                   YdbParams params);

typedef YdbStatus (*YdbSyncRetryable)(YdbSession session, void* data);

YdbStatus YdbRetryQuerySync(YdbQueryClient client, YdbSyncRetryable query,
                            void* data);

typedef YdbStatus (*YdbSyncRetryableNoSession)(YdbQueryClient client,
                                               void* data);

YdbStatus YdbRetryQuerySyncNoSession(YdbQueryClient client,
                                     YdbSyncRetryableNoSession query,
                                     void* data);
