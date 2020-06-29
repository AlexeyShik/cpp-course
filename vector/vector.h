#ifndef MY_VECTOR_MY_VECTOR_H

#include <algorithm>
#include <cassert>
#include <cmath>

template<typename T>
class vector {
public:
    typedef T *iterator;
    typedef T const *const_iterator;

    vector() noexcept;                                          // O(1) nothrow
    vector(vector const &other);                                // O(N) strong
    vector &operator=(vector const &other);                     // O(N) strong

    ~vector() noexcept;                                         // O(N) nothrow

    T &operator[](size_t i) noexcept;                           // O(1) nothrow
    T const &operator[](size_t i) const noexcept;               // O(1) nothrow

    T *data() noexcept;                                         // O(1) nothrow
    T const *data() const noexcept;                             // O(1) nothrow
    size_t size() const noexcept;                               // O(1) nothrow

    T &front() noexcept;                                        // O(1) nothrow
    T const &front() const noexcept;                            // O(1) nothrow

    T &back() noexcept;                                         // O(1) nothrow
    T const &back() const noexcept;                             // O(1) nothrow
    void push_back(T const &);                                  // O(1)* strong
    void pop_back() noexcept;                                   // O(1) nothrow

    bool empty() const noexcept;                                // O(1) nothrow

    size_t capacity() const noexcept;                           // O(1) nothrow
    void reserve(size_t);                                       // O(N) strong
    void shrink_to_fit();                                       // O(N) strong

    void clear() noexcept;                                      // O(N) nothrow

    void swap(vector &) noexcept;                               // O(1) nothrow

    iterator begin() noexcept;                                  // O(1) nothrow
    iterator end() noexcept;                                    // O(1) nothrow

    const_iterator begin() const noexcept;                      // O(1) nothrow
    const_iterator end() const noexcept;                        // O(1) nothrow

    iterator insert(const_iterator pos, T const &);             // O(N) weak
    iterator erase(const_iterator pos);                         // O(N) weak
    iterator erase(const_iterator first, const_iterator last);  // O(N) weak

private:
    void change_capacity(size_t);                               // O(N) strong
    static T* copy(const T*, size_t, size_t);                   // O(N) strong
    static void free_data(T*, size_t);                          // O(N) nothrow

    T *data_;
    size_t size_;
    size_t capacity_;
};

template<typename T>
vector<T>::vector() noexcept :  data_(nullptr), size_(0), capacity_(0) {}

template<typename T>
void vector<T>::free_data(T* a, size_t last) {
    for (ptrdiff_t j = last - 1; j >= 0; --j) {
        a[j].~T();
    }
}

template<typename T>
T* vector<T>::copy(const T* from, size_t new_size, size_t new_capacity) {
    T *tmp = static_cast<T *>(operator new(new_capacity * sizeof(T)));
    size_t i = 0;
    try {
        for (; i < new_size; ++i) {
            new(tmp + i) T(from[i]);
        }
    } catch (...) {
        free_data(tmp, i);
        operator delete(tmp);
        throw;
    }
    return tmp;
}

template<typename T>
vector<T>::vector(vector const &other) : vector() {
    if (other.empty()) {
        return;
    }
    data_ = copy(other.data_, other.size_, other.size_);
    size_ = capacity_ = other.size_;
}

template<typename T>
void vector<T>::swap(vector &other) noexcept {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}


template<typename T>
vector<T> &vector<T>::operator=(vector const &other) {
    vector<T> tmp(other);
    this->swap(tmp);
    return *this;
}

template<typename T>
vector<T>::~vector() noexcept {
    clear();
    operator delete(data_);
}

template<typename T>
size_t vector<T>::size() const noexcept {
    return size_;
}

template<typename T>
bool vector<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
size_t vector<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
T *vector<T>::data() noexcept {
    return data_;
}

template<typename T>
T const *vector<T>::data() const noexcept {
    return data_;
}

template<typename T>
void vector<T>::change_capacity(size_t new_capacity) {
    assert(new_capacity >= size_);
    if (capacity_ == new_capacity) {
        return;
    }
    T *tmp = copy(data_, size_, new_capacity);
    capacity_ = new_capacity;
    free_data(data_, size_);
    operator delete(data_);
    data_ = tmp;
}

template<typename T>
void vector<T>::reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        change_capacity(new_capacity);
    }
}

template<typename T>
void vector<T>::shrink_to_fit() {
    if (!empty()) {
        change_capacity(size_);
    } else {
        operator delete(data_);
        data_ = nullptr;
    }
}

template<typename T>
void vector<T>::clear() noexcept {
    free_data(data_, size_);
    size_ = 0;
}

template<typename T>
T &vector<T>::operator[](size_t i) noexcept {
    return data_[i];
}

template<typename T>
T const &vector<T>::operator[](size_t i) const noexcept {
    return data_[i];
}

template<typename T>
T &vector<T>::front() noexcept {
    return data_[0];
}

template<typename T>
T const &vector<T>::front() const noexcept {
    return data_[0];
}


template<typename T>
T &vector<T>::back() noexcept {
    return data_[size_ - 1];
}

template<typename T>
T const &vector<T>::back() const noexcept {
    return data_[size_ - 1];
}

template<typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return data_;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return data_;
}

template<typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
    return data_ + size_;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
    return data_ + size_;
}

template<typename T>
void vector<T>::push_back(T const &value) {
    if (size_ == capacity_) {
        T copy = value;
        change_capacity(capacity_ ? 2 * capacity_ : 1);
        new(data_ + size_) T(copy);
    } else {
        new(data_ + size_) T(value);
    }
    ++size_;
}

template<typename T>
void vector<T>::pop_back() noexcept {
    data_[--size_].~T();
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, T const &value) {
    ptrdiff_t insertion_point = pos - data_;
    push_back(value);
    for (ptrdiff_t curr = size_ - 1; curr - 1 >= insertion_point; --curr) {
        std::swap(data_[curr - 1], data_[curr]);
    }
    return data_ + insertion_point;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator pos) {
    return erase(pos, pos + 1);
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator first, vector::const_iterator last) {
    if (last - first > 0) {
        const ptrdiff_t n_deletions = last - first;
        for (ptrdiff_t curr = last - data_; curr < size_; ++curr) {
            std::swap(data_[curr - n_deletions], data_[curr]);
        }
        for (ptrdiff_t times = 0; times < n_deletions; ++times) {
            pop_back();
        }
    }
    return data_ + (first - data_);
}

#define MY_VECTOR_MY_VECTOR_H

#endif //MY_VECTOR_MY_VECTOR_H