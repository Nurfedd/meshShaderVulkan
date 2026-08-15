#pragma once
#include <utility>   // std::exchange, std::move
#include <cstddef>   // nullptr_t
#include <type_traits>

template <typename T>
class TRefCountPtr {
public:
    // --- Constructors ---

    TRefCountPtr() = default;
    TRefCountPtr(std::nullptr_t) {}

    // Construct from a raw pointer.
    // bAddRef = true (default): take a reference on an existing object.
    // bAddRef = false: used when taking ownership of a freshly created
    // object whose refcount starts at 0 (or was pre-incremented by a
    // factory/pool), so this pointer becomes the first owner without
    // an extra AddRef/Release pair.
    explicit TRefCountPtr(T* InPtr, bool bAddRef = true)
        : m_ptr(InPtr)
    {
        if (m_ptr && bAddRef) {
            m_ptr->AddRef();
        }
    }

    // Copy: we share ownership of the resource, so AddRef.
    TRefCountPtr(const TRefCountPtr& Other)
        : m_ptr(Other.m_ptr)
    {
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    // Copy from a convertible/derived type (e.g. TRefCountPtr<RHITexture> -> TRefCountPtr<RHIResource>)
    template <typename U>
    TRefCountPtr(const TRefCountPtr<U>& Other)
        : m_ptr(Other.Get())
    {
        static_assert(std::is_convertible_v<U*, T*>, "Incompatible types for TRefCountPtr");
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    // Move: steal the pointer, no AddRef/Release needed.
    TRefCountPtr(TRefCountPtr&& Other) noexcept
        : m_ptr(std::exchange(Other.m_ptr, nullptr))
    {
    }

    template <typename U>
    TRefCountPtr(TRefCountPtr<U>&& Other) noexcept
        : m_ptr(Other.Detach())
    {
        static_assert(std::is_convertible_v<U*, T*>, "Incompatible types for TRefCountPtr");
    }

    // --- Destructor ---

    ~TRefCountPtr()
    {
        if (m_ptr) {
            m_ptr->Release();
        }
    }


    TRefCountPtr& operator=(const TRefCountPtr& Other)
    {
        if (this != &Other) {
            T* OldPtr = m_ptr;
            m_ptr = Other.m_ptr;
            if (m_ptr) {
                m_ptr->AddRef();
            }
            if (OldPtr) {
                OldPtr->Release(); // release last: safe even if aliasing occurs
            }
        }
        return *this;
    }

    TRefCountPtr& operator=(TRefCountPtr&& Other) noexcept
    {
        if (this != &Other) {
            T* OldPtr = m_ptr;
            m_ptr = std::exchange(Other.m_ptr, nullptr);
            if (OldPtr) {
                OldPtr->Release();
            }
        }
        return *this;
    }

    TRefCountPtr& operator=(T* InPtr)
    {
        if (m_ptr != InPtr) {
            T* OldPtr = m_ptr;
            m_ptr = InPtr;
            if (m_ptr) {
                m_ptr->AddRef();
            }
            if (OldPtr) {
                OldPtr->Release();
            }
        }
        return *this;
    }

    TRefCountPtr& operator=(std::nullptr_t)
    {
        Reset();
        return *this;
    }

    // --- Accessors ---

    T* Get() const { return m_ptr; }

    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

    explicit operator bool() const { return m_ptr != nullptr; }

    bool operator==(const TRefCountPtr& Other) const { return m_ptr == Other.m_ptr; }
    bool operator!=(const TRefCountPtr& Other) const { return m_ptr != Other.m_ptr; }
    bool operator==(std::nullptr_t) const { return m_ptr == nullptr; }
    bool operator!=(std::nullptr_t) const { return m_ptr != nullptr; }


    // Releases the current reference (if any) without keeping the pointer.
    void Reset()
    {
        if (m_ptr) {
            T* OldPtr = std::exchange(m_ptr, nullptr);
            OldPtr->Release();
        }
    }

    // Releases ownership WITHOUT calling Release() -> transfers ownership
    // to code that will manage the refcount itself (e.g. C API, interop).
    [[nodiscard]] T* Detach()
    {
        return std::exchange(m_ptr, nullptr);
    }

    // Equivalent of a raw T** output parameter, used for factory-style
    // functions like CreateResource(TRefCountPtr<T>& OutPtr). Releases
    // any existing reference first to avoid leaking it.
    T** GetInitReference()
    {
        Reset();
        return &m_ptr;
    }

private:
    T* m_ptr = nullptr;

    // Lets TRefCountPtr<U> access m_ptr of TRefCountPtr<T> (conversion constructors)
    template <typename U> friend class TRefCountPtr;
};

// make_shared-style helper: creates the object and takes the first
// reference without a redundant AddRef (the object starts at refcount 0).
template <typename T, typename... Args>
TRefCountPtr<T> MakeRefCount(Args&&... args)
{
    return TRefCountPtr<T>(new T(std::forward<Args>(args)...), /*bAddRef=*/false);
}