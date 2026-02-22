#include <utility>
#include <cstddef>

template<typename T> class shared_ptr;
template<typename T> class weak_ptr;

template<typename T>
struct ControlBlock {
    size_t strong_count;
    size_t weak_count;
    T object;

    // take in a list of args of any type
    // forward all arguments to T's constructor
    // Args = types, args = values
    template<typename... Args>
    ControlBlock(Args&&... args)
        : strong_count(1),
          weak_count(0),
          object(std::forward<Args>(args)...) {} //in-place construction of T in cb
};

template<typename T>
class shared_ptr {
private:
    T* ptr;
    ControlBlock<T>* control;

    void release() {
        if (!control) return;
        if (--control->strong_count== 0) {
            ptr->~T();

            if (control->weak_count== 0) {
                delete control;
            }
        }
        ptr = nullptr;
        control = nullptr;
    }
    
public:
    // Default Constructor
    shared_ptr() noexcept : ptr(nullptr), control(nullptr) {};

    // Constructor for make_shared
    explicit shared_ptr(ControlBlock<T>* cb) : ptr(&cb->object), control(cb) {}

    // Copy Constructor
    shared_ptr(const shared_ptr& other)
        : ptr(other.ptr), control(other.control) {
        if (control) ++control->strong_count;
    }

    // Move Constructor
    shared_ptr(shared_ptr&& other) noexcept
        : ptr(other.ptr), control(other.control) {
        other.ptr = nullptr;
        other.control = nullptr;
    }

    // Copy Assignment
    shared_ptr& operator=(const shared_ptr& other) {
        if (this == &other) return *this;

        // decrease current ownership
        release();

        // copy from other
        ptr = other.ptr;
        control = other.control;

        // increase new ownership
        if (control) ++control->strong_count;

        return *this;
    }

    // Move Assignment
    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this == &other) return *this;

        // decrease current ownership
        release();

        // steal from other
        ptr = other.ptr;
        control = other.control;

        other.ptr = nullptr;
        other.control = nullptr;

        return *this;
    }

    // Destructor
    ~shared_ptr() {
        release();
    }

    // Modifiers
    void reset() noexcept {
        release();
    }
    void swap (shared_ptr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(control, other.control);
    }

    // Observers
    T* get() const noexcept { return ptr; }
    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }
    size_t use_count() const {
        return control ? control->strong_count : 0;
    }
    bool unique() const {
        return use_count() == 1;
    }
    explicit operator bool() const {
        return ptr != nullptr;
    }

    // give weak_ptr and make_shared access to private members
    friend class weak_ptr<T>;

    template<typename U, typename... Args>
    friend shared_ptr<U> make_shared(Args&&...);
};

template <typename T>
class weak_ptr {
private:
    ControlBlock<T>* control;

    void release() {
        if (!control) return;
        if (--control->weak_count == 0 && control->strong_count == 0) {
            delete control;
        }
        control = nullptr;
    }

public:
    // Default Constructor
    weak_ptr() noexcept : control(nullptr) {};

    // Copy Constructor
    weak_ptr(const weak_ptr& other)
        : control(other.control) {
        if (control) ++control->weak_count;
    }

    // Copy Constructor from shared ptr
    weak_ptr(const shared_ptr<T>& sp)
        : control(sp.control) {
        if (control) ++control->weak_count;
    }

    // Move Constructor
    weak_ptr(weak_ptr&& other) noexcept
        : control(other.control) {
        other.control = nullptr;
    }

    // Destructor
    ~weak_ptr() {
        release();
    }

    // Copy Assignment
    weak_ptr& operator=(const weak_ptr& other) {
        if (this == &other) return *this;

        release();

        // copy from other
        control = other.control;

        // increase new ownership
        if (control) ++control->weak_count;

        return *this;
    }

    // Move Assignment
    weak_ptr& operator=(weak_ptr&& other) noexcept {
        if (this == &other) return *this;

        release();

        // steal from other
        control = other.control;
        other.control = nullptr;

        return *this;
    }

    // Modifiers
    void reset () noexcept {
        release();
    }
    void swap (weak_ptr& other) noexcept {
        std::swap(control, other.control);
    }

    // Observers
    size_t use_count() const {
        return control ? control->strong_count: 0;
    }
    bool expired() const {
        return !control || control->strong_count == 0;
    }
    shared_ptr<T> lock() const {
        if (expired()) return shared_ptr<T>();
        ++control->strong_count;
        return shared_ptr<T>(control);
    }
};

// single heap allocation
template<typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto* cb = new ControlBlock<T>(std::forward<Args>(args)...);
    return shared_ptr<T>(cb);
}