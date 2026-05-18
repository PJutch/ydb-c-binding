#ifndef YDB_C_SDK_QUERY_H_
#define YDB_C_SDK_QUERY_H_

#include "driver.h"
#include "params.h"
#include "results.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbQueryClient)

YdbQueryClient YdbCreateQueryClient(YdbDriver driver);
void YdbDestroyQueryClient(YdbQueryClient client);

YDB_C_SDK_OPAQUE_STRUCT(YdbSession)

void YdbDestroySession(YdbSession session);

YDB_C_SDK_RESULT(CreateSessionResult)

YdbSession YdbCreateSessionResultGetSession(YdbCreateSessionResult result);

YdbAsyncCreateSessionResult YdbCreateSession(YdbQueryClient client);

typedef enum YdbTxMode {
    YDB_TX_SERIALIZABLE_RW,
    YDB_TX_ONLINE_RO,
    YDB_TX_STALE_RO,
    YDB_TX_SNAPSHOT_RO,
    YDB_TX_SNAPSHOT_RW,
} YdbTxMode;

YDB_C_SDK_OPAQUE_STRUCT(YdbTransaction)

void YdbDestroyTransaction(YdbTransaction transaction);

#define NULL_TRANSACTION (YdbTransaction){NULL};

YDB_C_SDK_OPAQUE_STRUCT(YdbTx);

YdbTx YdbNoTx();
YdbTx YdbTransactionTx(YdbTransaction transaction, bool commit);
YdbTx YdbBeginTx(YdbTxMode mode, bool commit, bool allow_inconsistent_reads);
void YdbDestroyTx(YdbTx tx);

YDB_C_SDK_RESULT(BeginTransactionResult)

YdbTransaction
YdbBeginTransactionResultGetTransaction(YdbBeginTransactionResult result);

YdbAsyncBeginTransactionResult
YdbBeginTransaction(YdbSession session, YdbTxMode mode,
                    bool allow_inconsistent_reads);
                
YDB_C_SDK_RESULT(CommitResult)

YdbAsyncCommitResult YdbCommit(YdbTransaction transaction);

YdbAsyncQueryResult YdbExecuteQuery(YdbSession session, char* query, YdbTx tx,
                                    YdbParams params);

YdbTransaction YdbQueryTransaction(YdbQueryResult result);

#ifdef __cplusplus
}
#endif

#endif