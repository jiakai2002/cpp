#include <iostream>
#include <utility>
#include <stdexcept>
#include <new>

template<typename T>
class vector {
private: 
	T* data;
	size_t size;
	size_t capacity;

	void reallocate(size_t new_capacity) {
		// allocate raw memory
		T* new_data = static_cast<T*>(operator new(new_capacity*sizeof(T)));

		for (size_t i=0; i<size; i++) {
			// move construct existing elements
			// call T's move constructor to steal resource from old object
			new (new_data+i) T(std::move(data[i]));
			// destroy old object
			data[i].~T();
		}

		// free old memory
		operator delete(data);
		data = new_data;
		capacity = new_capacity;
	}

public:
	// 1) Constructors
	// Default Constructor
	vector() : data(nullptr), size(0), capacity(0) {}

	// Constructor
	vector(size_t n) : size(0), capacity(n) {
		data = static_cast<T*>(operator new(capacity*sizeof(T)));
	}

	// Copy Constructor
	vector(const vector& other) {
		size = other.size;
		capacity = other.capacity;
		data = static_cast<T*>(operator new(capacity*sizeof(T)));
		for (size_t i=0; i<size; i++) {
			new (data+i) T(other.data[i]);
		}
	}

	// Move Constructor
	vector(vector&& other) noexcept {
		data = other.data;
		size = other.size;
		capacity = other.capacity;

		other.data = nullptr;
		other.size = 0;
		other.capacity = 0;
	}
	
	// 2) Assignments
	// Copy assignment
	vector& operator=(const vector& other) {
		if (this != &other) {
			// destroy and free
			for (size_t i=0; i<size; i++) {
				data[i].~T();
			}
			operator delete(data);

			// allocate new
			size = other.size;
			capacity = other.capacity;
			data = static_cast<T*>(operator new(capacity*sizeof(T)));

			//copy elements
			for (size_t i=0; i<size; i++) {
				new (data+i) T(other.data[i]);
			}
		}
		return *this;
	}

	// Move Assignment
	vector& operator=(vector&& other) noexcept {
		if (this != &other) {
			// destroy and free
			for (size_t i=0; i<size; i++) {
				data[i].~T();
			}
			operator delete(data);

			// steal resources
			data = other.data;
			size = other.size;
			capacity = other.capacity;

			other.data = nullptr;
			other.size = 0;
			other.capacity = 0;
		}
		return *this;
	}

	// 3) Destructor
	~vector() {
		for (size_t i=0; i<size; i++) {
			data[i].~T();
		}
		operator delete(data);
	}

	// 4) Functions
	// Insert
	void push_back(const T& value) {
		if (size == capacity) {
			reallocate(capacity == 0 ? 1 : capacity * 2);
		}
		new (data+size) T(value);
		size++;
	}

	void push_back(T&& value) {
		if (size == capacity) {
			reallocate(capacity == 0 ? 1 : capacity * 2);
		}
		new (data+size) T(std::move(value));
		size++;
	}

	// Delete
	void pop() {
		if (size == 0) throw std::out_of_range("Empty vector");
		size--;
		data[size].~T();
	}

	// Access
	T& operator[](size_t i) {return data[i];}
	const T& operator[](size_t i) const {return data[i];}
	
	// Size/Capacity
	size_t getSize() { return size; }
	size_t getCapacity() { return capacity; }
};
