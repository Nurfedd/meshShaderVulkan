#pragma once

#include <type_traits>

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

#define ENUM_CLASS_FRIEND_FLAGS(Enum)                                          \
friend inline Enum operator|(Enum a, Enum b)                            \
{                                                                        \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(static_cast<T>(a) | static_cast<T>(b));    \
}                                                                        \
friend inline Enum operator&(Enum a, Enum b)                            \
{                                                                        \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(static_cast<T>(a) & static_cast<T>(b));    \
}                                                                        \
friend inline Enum operator^(Enum a, Enum b)                            \
{                                                                        \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(static_cast<T>(a) ^ static_cast<T>(b));    \
}                                                                        \
friend inline Enum operator~(Enum a)                                    \
{                                                                        \
    using T = std::underlying_type_t<Enum>;                             \
    return static_cast<Enum>(~static_cast<T>(a));                       \
}                                                                        \
friend inline Enum& operator|=(Enum& a, Enum b)                         \
{                                                                        \
    a = a | b;                                                          \
    return a;                                                          \
}                                                                        \
friend inline Enum& operator&=(Enum& a, Enum b)                         \
{                                                                        \
    a = a & b;                                                          \
    return a;                                                          \
}

template <typename Enum>
void RemoveFlag(Enum& a, Enum b) {
    a &= ~b;
}