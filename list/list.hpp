#pragma once
#include <cstddef>
#include <utility>
#include <stdexcept>

template<typename T>
class list {
private:
    // stored on heap
    struct Node {
        T value;
        Node* next;
        Node* prev;
        Node(const T& val) : value(val), next(nullptr), prev(nullptr) {}
        Node(T&& val) : value(std::move(val)), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t sz;

public:
    list() : head(nullptr), tail(nullptr), sz(0) {}
    ~list() { clear(); }

    // Iterator - pointer wrapper to support iterator for-loop 
    struct iterator {
        Node* ptr;
        iterator(Node* p) : ptr(p) {}
        T& operator*() { return ptr->value; }
        iterator& operator++() { ptr = ptr->next; return *this; }
        iterator& operator--() { ptr = ptr->prev; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    // Capacity
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }

    // Modifiers
    void push_back(const T& val) {
        Node* node = new Node(val);
        if (!tail) head = tail = node;
        else { tail->next = node; node->prev = tail; tail = node; }
        ++sz;
    }

    void push_back(T&& val) {
        Node* node = new Node(std::move(val));
        if (!tail) head = tail = node;
        else { tail->next = node; node->prev = tail; tail = node; }
        ++sz;
    }

    void push_front(const T& val) {
        Node* node = new Node(val);
        if (!head) head = tail = node;
        else { node->next = head; head->prev = node; head = node; }
        ++sz;
    }

    void push_front(T&& val) {
        Node* node = new Node(std::move(val));
        if (!head) head = tail = node;
        else { node->next = head; head->prev = node; head = node; }
        ++sz;
    }

    void pop_back() {
        if (!tail) return;
        Node* tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete tmp;
        --sz;
    }

    void pop_front() {
        if (!head) return;
        Node* tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete tmp;
        --sz;
    }

    iterator insert(iterator pos, const T& val) {
        if (pos.ptr == nullptr) { push_back(val); return iterator(tail); }
        Node* node = new Node(val);
        node->next = pos.ptr;
        node->prev = pos.ptr->prev;
        if (pos.ptr->prev) pos.ptr->prev->next = node;
        else head = node;
        pos.ptr->prev = node;
        ++sz;
        return iterator(node);
    }

    iterator erase(iterator pos) {
        if (pos.ptr == nullptr) return end();
        Node* node = pos.ptr;
        iterator ret(node->next);
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
        delete node;
        --sz;
        return ret;
    }

    void clear() {
        Node* curr = head;
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
        head = tail = nullptr;
        sz = 0;
    }

    // Swap
    void swap(list& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(sz, other.sz);
    }
};