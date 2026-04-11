#ifndef YDB_C_SDK_QUERY_H_
#define YDB_C_SDK_QUERY_H_

#include "driver.h"
#include "params.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TQueryClient TQueryClient;

TQueryClient* CreateQueryClient(TDriver* driver);
void DestroyQueryClient(TQueryClient* client);

typedef void* TStatus;

void DestroyStatus(TStatus status);

bool IsSuccess(TStatus status);
bool IsTransportError(TStatus status);

char* GetErrorMessage(TStatus status);
void DestroyErrorMessage(char* message);

typedef void* TQueryResult;

void DestroyResult(TQueryResult result);

// Doesn't copy, don't destroy both
TStatus AsStatus(TQueryResult result);

typedef void* TSession;

typedef enum TxMode {
    TX_SERIALIZABLE_RW,
    TX_ONLINE_RO,
    TX_STALE_RO,
    TX_SNAPSHOT_RO,
    TX_SNAPSHOT_RW,
    TX_TRANSACTION,
} TxMode;

typedef void* Transaction;

typedef struct TTx {
    TxMode mode;
    bool commit;
    bool allow_inconsistent_reads;
    Transaction transaction;
} TTx;

TQueryResult ExecuteQuerySync(TSession session, char* query, TTx* tx, TParams params);

typedef TStatus (*SyncRetryable) (TSession session, void* data);

TStatus RetryQuerySync(TQueryClient* client, SyncRetryable query, void* data);

#ifdef __cplusplus
}
#endif

#endif