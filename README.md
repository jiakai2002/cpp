# C++ Custom Implementations

Custom implementations of core C++ data structures and smart pointers from scratch, with manual memory management and full test coverage via Google Test.

---

## `unique_ptr<T>`

A smart pointer enforcing single ownership — copy is deleted, only move is allowed.

**Implemented:** Default constructor, raw pointer constructor, destructor, move constructor, move assignment, `reset`, `release`, `swap`, `get`, `operator*`, `operator->`, `operator bool`

**Key learnings:** Deleting copy operations at the compiler level enforces ownership semantics. `release()` transfers raw pointer ownership without destroying the resource, while `reset()` destroys the old resource before taking a new one. Using `explicit` on the constructor prevents accidental implicit conversions from raw pointers.

---

## `shared_ptr<T>` / `weak_ptr<T>`

Reference-counted shared ownership using a unified `ControlBlock<T>` that holds the object, strong count, and weak count in a single heap allocation.

**Implemented:** Default constructor, control block constructor, copy/move constructors and assignments, destructor, `reset`, `swap`, `get`, `use_count`, `unique`, `operator bool`, `make_shared`; `weak_ptr` with `lock`, `expired`, `use_count`, copy/move

**Key learnings:** `make_shared` does a single allocation (object + ref counts together) vs. two separate allocations in a naive implementation. The control block outlives the object — it's only freed when both strong and weak counts hit zero. `weak_ptr::lock()` safely promotes to a `shared_ptr` only if the object is still alive, avoiding dangling pointer access.

---

## `vector<T>`

A dynamic array using raw memory (`operator new`/`operator delete`) with placement new for in-place construction, doubling capacity on reallocation.

**Implemented:** Default constructor, sized constructor, copy/move constructors and assignments, destructor, `push_back` (copy & move), `pop`, `operator[]`, `getSize`, `getCapacity`, iterator

**Key learnings:** Separating allocation (`operator new`) from construction (placement `new`) avoids calling default constructors on uninitialized memory. During reallocation, existing elements are move-constructed into new memory then explicitly destroyed in the old buffer — this is both exception-aware and avoids unnecessary copies. Elements must be manually destroyed via `~T()` before freeing raw memory.

---

## `list<T>`

A doubly linked list of heap-allocated `Node` structs with `head`/`tail` sentinels and bidirectional iteration.

**Implemented:** Default constructor, destructor, `push_back`/`push_front` (copy & move), `pop_back`/`pop_front`, `insert`, `erase`, `clear`, `swap`, `size`, `empty`, bidirectional iterator

**Key learnings:** Each node is independently heap-allocated, so no reallocation is needed on insert/erase — only pointer rewiring. `clear()` walks the chain and deletes each node individually. The iterator wraps a raw `Node*` and implements `operator*`, `operator++`/`--`, and `operator!=` to slot into range-based for loops.

---

## `string`

A heap-managed character array with null terminator, supporting dynamic growth and common string operations.

**Implemented:** Default constructor, C-string constructor, copy/move constructors and assignments, destructor, `push_back`, `pop_back`, `append`, `reserve`, `shrink_to_fit`, `clear`, `find` (char & substring), `substr`, `compare`, `at`, `operator[]`, `c_str`, `empty`

**Key learnings:** Capacity doubles on `push_back` to amortize reallocation cost. `shrink_to_fit` reallocates to exactly `size` to reclaim unused memory. `find` uses a naive O(n·m) substring scan — sufficient for this implementation. The null terminator is maintained manually after every mutation. Copy-and-swap idiom (commented out) is noted as an alternative for strong exception safety.

---

## Testing

All structures are tested with [Google Test](https://github.com/google/googletest). Each `.hpp` has a corresponding `test.cpp` covering construction, copy/move semantics, edge cases, and memory safety.
