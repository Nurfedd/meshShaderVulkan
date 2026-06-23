#define ENUM_CLASS_FLAGS(Enum)                                          \
inline Enum operator|(Enum a, Enum b)                                   \
{                                                                       \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(static_cast<T>(a) | static_cast<T>(b));   \
}                                                                       \
                                                                        \
inline Enum operator&(Enum a, Enum b)                                   \
{                                                                       \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(static_cast<T>(a) & static_cast<T>(b));   \
}                                                                       \
                                                                        \
inline Enum operator^(Enum a, Enum b)                                   \
{                                                                       \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(static_cast<T>(a) ^ static_cast<T>(b));   \
}                                                                       \
                                                                        \
inline Enum operator~(Enum a)                                           \
{                                                                       \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(~static_cast<T>(a));                       \
}                                                                       \
                                                                        \
inline Enum& operator|=(Enum& a, Enum b)                                \
{                                                                       \
    a = a | b;                                                          \
    return a;                                                          \
}                                                                       \
                                                                        \
inline Enum& operator&=(Enum& a, Enum b)                                \
{                                                                       \
    a = a & b;                                                          \
    return a;                                                          \
}