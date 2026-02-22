#include <utility>
#include <cstddef>

template<typename T>
class unique_ptr {
private:
    T* ptr;
public:
    // Default Constructor
    unique_ptr() noexcept
        : ptr(nullptr) {};

    // Construct from raw pointer 
    explicit unique_ptr(T* p) noexcept
        : ptr(p) {};

    // Destructor
    ~unique_ptr() {
        delete(ptr);
    }

    // Delete Copy operations - we allow only one unique ptr per object
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // Move Constructor 
    unique_ptr(unique_ptr&& other) noexcept {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    // Move Assignment
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Modifiers
    void reset(T* new_ptr = nullptr) noexcept {
        delete ptr;
        ptr = new_ptr;
    }
    void swap(unique_ptr& other) noexcept {
        std::swap(ptr, other.ptr);
    }

    // Observers
    T& operator*() const noexcept { return *ptr; }
    T& operator->() const noexcept { return *ptr; }
    T* get() const noexcept { return ptr; }
    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
    explicit operator bool() const noexcept {
        return ptr!=nullptr;
    }
};