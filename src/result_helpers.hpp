#ifndef YDB_RESULT_HELPERS_HPP_
#define YDB_RESULT_HELPERS_HPP_

#define YDB_C_SDK_RESULT_IMPL(name_no_ydb, cpp_sdk_type, async_cpp_sdk_type)   \
    void YdbDestroy##name_no_ydb(Ydb##name_no_ydb result) {                    \
        delete YdbPtrFromOpaque<cpp_sdk_type>(result);                         \
    }                                                                          \
                                                                               \
    /* Doesn't copy, don't destroy both */                                     \
    YdbStatus Ydb##name_no_ydb##AsStatus(Ydb##name_no_ydb result) {            \
        auto* as_status = static_cast<NYdb::TStatus*>(                         \
            YdbPtrFromOpaque<cpp_sdk_type>(result));                           \
        return PTR_TO_OPAQUE(as_status);                                       \
    }                                                                          \
                                                                               \
    void YdbDestroyAsync##name_no_ydb(YdbAsync##name_no_ydb result) {          \
        delete YdbPtrFromOpaque<async_cpp_sdk_type>(result);                   \
    }                                                                          \
                                                                               \
    /* Destroys async result, no need to call  YdbDestroyAsync* */             \
    Ydb##name_no_ydb YdbGetSync##name_no_ydb(YdbAsync##name_no_ydb result) {   \
        Ydb##name_no_ydb sync_result = TO_NEW_OPAQUE(                          \
            cpp_sdk_type,                                                      \
            YdbFromOpaque<async_cpp_sdk_type>(result).ExtractValueSync());         \
        YdbDestroyAsync##name_no_ydb(result);                                  \
        return sync_result;                                                    \
    }

#endif