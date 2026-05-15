#ifndef YDB_RESULT_HELPERS_HPP_
#define YDB_RESULT_HELPERS_HPP_

#define YDB_C_SDK_RESULT_IMPL(name_no_ydb, cpp_sdk_type)                       \
    void YdbDestroy##name_no_ydb(Ydb##name_no_ydb result) {                    \
        delete YdbPtrFromOpaque<cpp_sdk_type>(result);                         \
    }                                                                          \
                                                                               \
    /* Doesn't copy, don't destroy both */                                     \
    YdbStatus Ydb##name_no_ydb##AsStatus(Ydb##name_no_ydb result) {            \
        auto* as_status = static_cast<NYdb::TStatus*>(                         \
            YdbPtrFromOpaque<cpp_sdk_type>(result));                           \
        return PTR_TO_OPAQUE(as_status);                                       \
    }

#endif