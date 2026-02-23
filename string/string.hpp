#pragma once
#include <cstddef>
#include <cstring>
#include <utility>
#include <stdexcept>

class string {
    private:
        char* data;
        size_t size;
        size_t capacity;

        void reallocate(size_t new_capacity) {
            char* new_data = new char[new_capacity+1];

            // will lose data if new_capacity < size
            size_t copy_size = (size > new_capacity) ? new_capacity : size;
            std::memcpy(new_data, data, copy_size);

            new_data[copy_size] = '\0';

            delete[] data;
            data = new_data;
            size = copy_size;
            capacity = new_capacity;
        }

    public:
    // Constructors/Destructor
    string() noexcept : data(new char[1]), size(0), capacity(0) {
        data[0] = '\0';
    }

    // Construct from c string
    string(const char* s) {
        size = std::strlen(s);
        capacity = size;
        data = new char[capacity+1];
        std::memcpy(data, s, size+1);
    }

    // copy constructor
    string(const string& other) 
        : size(other.size), capacity(other.capacity) {
        data = new char[capacity+1];
        std::memcpy(data, other.data, size+1);
    }

    // move constructor
    string(string&& other) noexcept 
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = new char[1];
        other.data[0] = '\0';
        other.size = 0;
        other.capacity = 0;
    }

    ~string() {
        delete[] data;
    }

    // Assignment (Copy/Move)
    // Copy-and-swap for exception safety
    // Copy/Move construct + swap

    // string& operator=(string other) {
    //     swap(other);
    //     return *this;
    // }

    // Copy Assignment
    string& operator=(const string& other) {
        if (this == &other) return *this;

        delete[] data;

        size = other.size;
        capacity = other.capacity;

        data = new char[capacity + 1];
        std::memcpy(data, other.data, size + 1);

        return *this;
    }

    // Move Assignment
    string& operator=(string&& other) noexcept {
        if (this == &other) return *this;

        delete[] data;

        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }

    // Element access
    char& operator[](size_t index) { return data[index]; }
    const char& operator[](size_t index) const { return data[index]; }

    char& at(size_t index) {
        if (index >= size) throw std::out_of_range("string::at out of range");
        return data[index];
    }
    const char& at(size_t index) const {
        if (index >= size) throw std::out_of_range("string::at out of range");
        return data[index];
    }

    const char* c_str() const {
        return data ? data : "";
    }

    // Capacity
    bool empty() const { return size==0; }
    size_t getSize() const { return size ;}
    size_t getCapacity() const { return capacity ;}

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            reallocate(new_capacity);
        }
    }

    // Modifiers
    void swap(string& other) noexcept {
        std::swap(data,other.data);
        std::swap(size,other.size);
        std::swap(capacity,other.capacity);
    }

    void push_back(char c) {
        if (size==capacity) {
            size_t new_capacity = (capacity==0) ? 1 : capacity*2;
            reallocate(new_capacity);
        }
        data[size++] = c;
        data[size] = '\0';
    }

    void pop_back() {
        if (size > 0) {
            size--;
            data[size] = '\0';
        }
    }

    void append(const string& other) {
        if (size + other.size > capacity) {
            reserve(size + other.size);
        }
        std::memcpy(data+size, other.data, other.size);
        size += other.size;
        data[size] = '\0';
    }

    void clear() {
        size = 0;
        data[0] = '\0';
    }

    void shrink_to_fit() {
        if (capacity > size) {
            reallocate(size);
        }
    }

    // Search
    size_t find(const string& needle) const {
        if (needle.size == 0) return 0;
        if (needle.size > size) return npos;

        for (size_t i = 0; i <= size - needle.size; i++) {
            bool match = true;
            for (size_t j = 0; j < needle.size; j++) {
                if (data[i + j] != needle[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
        return npos;
    }

    size_t find(char c) const {
        for (size_t i=0; i<size; i++) {
            if (data[i] == c) return i;
        }
        return npos;
    }

    // Operations
    string substr(size_t pos, size_t len) const {
        if (pos > size) throw std::out_of_range("string::substr out of range");
        if (pos + len > size) len = size - pos;
        string res;
        res.reserve(len);
        for (size_t i=0; i<len; i++) {
            res.push_back(data[pos+i]);
        }
        return res;
    }

    int compare(const string& other) const {
        size_t min_len = (size < other.size) ? size : other.size;
        // memcpy returns <0 if first diff byte of data < other.data, etc.
        int cmp = std::memcmp(data, other.data, min_len);
        if (cmp != 0) return cmp;
        if (size == other.size) return 0;
        return (size < other.size) ? -1 : 1;
    }

    static constexpr size_t npos = static_cast<size_t>(-1);

};