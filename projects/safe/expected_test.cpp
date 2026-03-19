//
// Created by zaqwes on 27.03.2022.
//

//#include <iceoryx_utils/cxx/expected.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

[[nodiscard]] void* rtti() noexcept {
    //    auto a =typeid(int);
    // throw int();
    int* ptr = new int();
    (void)ptr;

    void* p = malloc(10);
    (void)p;

    return nullptr;
}

void g() { (void)rtti(); }

enum SystemError { no_error, out_of_memory };

// namespace {
// iox::cxx::expected<std::vector<int>, SystemError> makeVector() {
//     return iox::cxx::success<std::vector<int>>(std::vector<int>(0, 100));
// }
// }  // namespace

//========================================================================
//========================================================================
//========================================================================

namespace safe {
// Optional
// https://stackoverflow.com/questions/50271304/what-is-the-purpose-of-stdaligned-storage

template <class T, std::size_t N>
class embedded_vector {
    // properly aligned uninitialized storage for N T's
    // Size to pessimistic?
    // Should be inited it with {}?
    std::aligned_storage_t<sizeof(T), alignof(T)> data[N] = {};
    std::size_t m_size = 0;

public:
    // Create an object in aligned storage
    template <typename... Args>
    void emplace_back(Args&&... args) {
        //        if (m_size >= N) {  // possible error handling
        //            throw std::bad_alloc{};
        //        }

        // construct value in memory of aligned storage
        // using inplace operator new
        [[maybe_unused]] void* ptr = ::new (&data[m_size], std::nothrow) T(std::forward<Args>(args)...);
        ++m_size;
    }

    [[nodiscard]] bool emplace_back() {
        // construct value in memory of aligned storage
        // using inplace operator new
        void* ptr = ::new (&data[m_size]) T();

        if (ptr == nullptr) {
            return false;
        }
        ++m_size;

        return true;
    }

    // Access an object in aligned storage
    const T& operator[](std::size_t pos) const {
        // Note: std::launder is needed after the change of object model in P0137R1
        return *std::launder(reinterpret_cast<const T*>(&data[pos]));
    }

    // Destroy objects from aligned storage
    ~embedded_vector() {
        for (std::size_t pos = 0; pos < m_size; ++pos) {
            // Note: std::launder is needed after the change of object model in P0137R1
            std::destroy_at(std::launder(reinterpret_cast<T*>(&data[pos])));
        }
    }
};

template <typename T>
class optional {
private:
    std::aligned_storage_t<sizeof(T), alignof(T)> m_storage;
    bool m_valid;

public:
    constexpr optional() noexcept : m_valid(false) {}

    constexpr optional(const T& obj) noexcept(std::is_nothrow_copy_constructible<T>::value) : m_valid(false) {
        new (static_cast<void*>(&m_storage)) T(obj);
        m_valid = true;
    }

    constexpr const T& operator*() const { return *static_cast<const T*>(static_cast<const void*>(&m_storage)); }

    constexpr T& operator*() { return *static_cast<T*>(static_cast<void*>(&m_storage)); }

    ~optional() {
        if (m_valid) operator*().~T();
    }
    // Much more, to obey the Rule Of Five and add more functionality...
};
}  // namespace safe

//========================================================================
//========================================================================
//========================================================================

class Obj {
public:
    virtual ~Obj() = default;
};

class ObjImpl final : public Obj {
public:
    //    alignas(ObjImpl) unsigned char buf[sizeof(ObjImpl)];
    //    std::aligned_storage_t<sizeof(ObjImpl), alignof(ObjImpl)> data[1];
    //        return std::unique_ptr<ObjImpl>{ptr, [](ObjImpl* ptr) { ptr->ObjImplImpl(); }};  // It's bad with high
    //        prob ObjImpl* tptr = new(buf) ObjImpl;
    // placement new: construct the value in the statically allocated space
    // <- can this whole expression throw if the constructor of T doesn’t throw?

    // Can't use sizeof here
    //    template <class... Args>
    //    static std::optional<ObjImpl> create(Args&&... args, std::aligned_storage_t<sizeof(ObjImpl),
    //    alignof(ObjImpl)>& buf) noexcept {
    //        return ObjImpl(std::forward<Args>(args)...);  // Alarm! Link heap!
    //    }

    template <class... Args>
    static std::optional<ObjImpl> create(Args&&... args) noexcept {
        return ObjImpl(std::forward<Args>(args)...);  // Alarm! Link heap!
    }

    // private:
    ObjImpl() noexcept = default;

    //    static_assert(std::is_nothrow_default_constructible_v<ObjImpl>, "");  // can't be here

    ~ObjImpl() final { std::cout << "dtor" << std::endl; }
};

static_assert(std::is_nothrow_default_constructible_v<ObjImpl>);

// Can't call sizeof inside of class
static std::shared_ptr<Obj> create(std::aligned_storage_t<sizeof(ObjImpl), alignof(ObjImpl)>& buf) noexcept {
    Obj* obj_ptr = ::new (&buf) ObjImpl{};
    if (obj_ptr == nullptr) {
        return nullptr;
    }

    // https://stackoverflow.com/questions/40137660/boost-intrusive-pointer
    // But how to call dtor?
    return std::shared_ptr<Obj>(obj_ptr, [](auto ptr) { ptr->~Obj(); });  // Where is atomic ptr? Heap?
}

template <typename T, typename U, int N = 8>
class SingleThreadProvider {
public:
    static_assert(std::is_base_of_v<U, T>);
    std::aligned_storage_t<sizeof(T), alignof(T)> buf[N] = {};  // Uninitialized?
    std::array<bool, N> used_{};                                //  fill with false?

    U* get() {
        for (int i = 0; i < N; ++i) {
        }
        return nullptr;
    }

    ~SingleThreadProvider() {
        // delete all used
    }
};

// Related:
//   "Type punning in modern C++ - Timur Doumler - CppCon 2019"

int main() {
    // No heap
    //   https://barrgroup.com/embedded-systems/how-to/polymorphism-no-heap-memory
    //   Objects pool
    //   https://www.youtube.com/watch?v=j2AgjFSFgRc&t=249s&ab_channel=MeetingCpp
    // No exception
    // No rtti

    //    auto v = makeVector();
    auto f = new int();  // No malloc?
                         //    static_cast<void>(v);
    static_cast<void>(f);

    g();

    {
        std::aligned_storage_t<sizeof(ObjImpl), alignof(ObjImpl)> buf;
        auto shared_ptr = create(buf);
    }

    {
        safe::embedded_vector<ObjImpl, 8> objs;
        bool success = objs.emplace_back();
        static_cast<void>(success);
    }

    //    auto z = ObjImpl::create();
    //    static_cast<void>(z);

    int* p = static_cast<int*>(malloc(10));
    p[0] = 1;
    return p[0];
}