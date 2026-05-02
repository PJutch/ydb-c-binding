#ifndef YDB_C_SDK_HELPERS_HPP_
#define YDB_C_SDK_HELPERS_HPP_

#define FROM_OPAQUE(type, opaque) (*static_cast<type*>((opaque).data))
#define PTR_FROM_OPAQUE(type, opaque) (static_cast<type*>((opaque).data))

#define TO_NEW_OPAQUE(underlying_type, ...) {static_cast<void*>(new underlying_type{__VA_ARGS__})}

#endif