#ifndef YDB_C_SDK_HELPERS_HPP_
#define YDB_C_SDK_HELPERS_HPP_

template <typename T, typename Opaque> T& YdbFromOpaque(Opaque opaque) {
    return *static_cast<T*>((opaque).data);
}

template <typename T, typename Opaque> T* YdbPtrFromOpaque(Opaque opaque) {
    return static_cast<T*>((opaque).data);
}

#define TO_NEW_OPAQUE(underlying_type, ...)                                    \
    {static_cast<void*>(new underlying_type{__VA_ARGS__})}

#endif