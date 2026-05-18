#ifndef YDB_TX_HELPERS_HPP_
#define YDB_TX_HELPERS_HPP_

#include "ydb-c-sdk/query.h"

#include <ydb-cpp-sdk/client/query/query.h>

// expects mode != YdbX_TRANSACTION
NYdb::NQuery::TTxSettings YdbCreateTxSettings(YdbTxMode mode,
                                              bool allow_inconsistent_reads);

#endif