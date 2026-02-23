# C++ Data Structures & Smart Pointers

Custom implementations from scratch with manual memory management, tested with Google Test.

---

## `unique_ptr<T>`
Single-ownership smart pointer — copy deleted, move only.

**Operations:** constructor, destructor, move constructor/assignment, `reset`, `release`, `swap`, `get`, `operator*/->/bool`

**Notes:**
- Copy deleted at compiler level to enforce single ownership
- `explicit` constructor prevents accidental implicit conversion from raw pointers
- `release()` transfers ownership without destroying; `reset()` destroys then takes new pointer
- `delete` = destructor + free; used directly since unique_ptr does a single allocation

---

## `shared_ptr<T>` + `weak_ptr<T>`
Reference-counted shared ownership via a single `ControlBlock<T>` holding the object + strong/weak counts.

**Operations:** constructor, destructor, copy/move constructor/assignment, `reset`, `swap`, `use_count`, `unique`, `make_shared`; `weak_ptr`: `lock`, `expired`

**Notes:**
- Normal construction = two allocations (object + ref count); `make_shared` fuses them into one
- Control block outlives the object — freed only when both strong and weak counts reach zero
- `weak_ptr::lock()` safely promotes to `shared_ptr` only if the object is still alive
- `friend class` used to give `weak_ptr` and `make_shared` access to `shared_ptr`'s private members
- `return *this` in `operator=` enables assignment chaining (`a = b = c`)

---

## `vector<T>`
Dynamic array using raw memory with placement new and 2× growth on reallocation.

**Operations:** constructor, destructor, copy/move constructor/assignment, `push_back` (copy & move), `pop`, `operator[]`, `getSize/Capacity`, iterator

**Notes:**
- `operator new` = allocate only; placement `new` = construct in existing memory — avoids unnecessary default construction
- On reallocation, elements are move-constructed into new memory then explicitly destroyed via `~T()` in the old buffer
- `noexcept` on move operations prevents the compiler from falling back to copy

---

## `list<T>`
Doubly linked list of heap-allocated nodes with bidirectional iteration.

**Operations:** constructor, destructor, `push/pop_back/front` (copy & move), `insert`, `erase`, `clear`, `swap`, `size`, `empty`, bidirectional iterator

**Notes:**
- No reallocation on insert/erase — only pointer rewiring
- Iterator is a pointer wrapper implementing `*`, `++/--`, `!=` to plug into range-based for loops

---

## `string`
Heap-managed char array with null terminator and dynamic growth.

**Operations:** constructor, destructor, copy/move constructor/assignment, `push/pop_back`, `append`, `reserve`, `shrink_to_fit`, `clear`, `find`, `substr`, `compare`, `at`, `operator[]`, `c_str`, `empty`

**Notes:**
- Capacity doubles on `push_back`; `shrink_to_fit` reallocates to exactly `size`
- Null terminator maintained manually after every mutation
- Copy-and-swap idiom (copy/move construct + swap) offers stronger exception safety as an alternative assignment strategy
