#ifndef YDB_C_SDK_QUERY_H_
#define YDB_C_SDK_QUERY_H_

#include "driver.h"
#include "results.h"
#include "params.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbQueryClient)

YdbQueryClient YdbCreateQueryClient(YdbDriver driver);
void YdbDestroyQueryClient(YdbQueryClient client);

YDB_C_SDK_OPAQUE_STRUCT(YdbSession)

typedef enum YdbxMode {
    YDB_TX_SERIALIZABLE_RW,
    YDB_TX_ONLINE_RO,
    YDB_TX_STALE_RO,
    YDB_TX_SNAPSHOT_RO,
    YDB_TX_SNAPSHOT_RW,
    YDB_TX_TRANSACTION,
} YdbxMode;

YDB_C_SDK_OPAQUE_STRUCT(YdbTransaction)

typedef struct YdbTx {
    YdbxMode mode;
    bool commit;
    bool allow_inconsistent_reads;
    YdbTransaction transaction;
} YdbTx;

YdbQueryResult YdbExecuteQuerySync(YdbSession session, char* query, YdbTx* tx, YdbParams params);

typedef YdbStatus (*YdbSyncRetryable) (YdbSession session, void* data);

YdbStatus YdbRetryQuerySync(YdbQueryClient client, YdbSyncRetryable query, void* data);

#ifdef __cplusplus
}
#endif

#endif