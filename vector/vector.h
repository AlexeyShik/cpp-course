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

    iterator insert(iterator pos, T const &);                   // O(N) weak
    iterator insert(const_iterator pos, T const &);             // O(N) weak

    iterator erase(iterator pos);                               // O(N) weak
    iterator erase(const_iterator pos);                         // O(N) weak

    iterator erase(iterator first, iterator last);              // O(N) weak
    iterator erase(const_iterator first, const_iterator last);  // O(N) weak

private:
    void change_capacity(size_t);                               // O(N) strong
    iterator copy(vector<T> const &);                           // O(N) strong

private:
    T *data_;
    size_t size_;
    size_t capacity_;
};

template<typename T>
vector<T>::vector() noexcept :  data_(nullptr), size_(0), capacity_(0) {}

template<typename T>
typename vector<T>::iterator vector<T>::copy(vector<T> const &a) {
    T *tmp = static_cast<T *>(operator new(a.capacity() * sizeof(T)));
    size_t i = 0;
    try {
        for (; i < a.size(); ++i) {
            new(tmp + i) T(a[i]);
        }
    } catch (...) {
        while (i) {
            tmp[--i].~T();
        }
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
    try {
        data_ = copy(other);
    } catch (...) {
        data_ = nullptr;
        throw;
    }
    size_ = other.size();
    capacity_ = other.capacity();
    shrink_to_fit();
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
    tmp.shrink_to_fit();
    return *this;
}

template<typename T>
vector<T>::~vector() noexcept {
    clear();
    operator delete(data());
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
    return static_cast<T const *> (data_);
}

template<typename T>
void vector<T>::change_capacity(size_t new_capacity) {
    assert(new_capacity >= size());
    if (capacity() == new_capacity) {
        return;
    }
    capacity_ = new_capacity;
    T *tmp;
    try {
        tmp = copy(*this);
    } catch (...) {
        throw;
    }
    for (size_t j = 0; j < size(); ++j) {
        data_[j].~T();
    }
    operator delete(data_);
    data_ = tmp;
}

template<typename T>
void vector<T>::reserve(size_t new_capacity) {
    if (new_capacity > capacity()) {
        change_capacity(new_capacity);
    }
}

template<typename T>
void vector<T>::shrink_to_fit() {
    if (!empty()) {
        change_capacity(size());
    } else {
        operator delete(data_);
        data_ = nullptr;
    }
}

template<typename T>
void vector<T>::clear() noexcept {
    while (size()) {
        pop_back();
    }
}

template<typename T>
T &vector<T>::operator[](size_t i) noexcept {
    assert(i >= 0 && i < size());
    return data_[i];
}

template<typename T>
T const &vector<T>::operator[](size_t i) const noexcept {
    assert(i >= 0 && i < size());
    return data_[i];
}

template<typename T>
T &vector<T>::front() noexcept {
    assert(!empty());
    return data_[0];
}

template<typename T>
T const &vector<T>::front() const noexcept {
    assert(!empty());
    return data_[0];
}


template<typename T>
T &vector<T>::back() noexcept {
    assert(!empty());
    return data_[size() - 1];
}

template<typename T>
T const &vector<T>::back() const noexcept {
    assert(!empty());
    return data_[size() - 1];
}

template<typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return data();
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return data();
}

template<typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
    return data() + size();
}

template<typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
    return data() + size();
}

template<typename T>
void vector<T>::push_back(T const &value) {
    T copy = value;
    if (size() == capacity()) {
        change_capacity(capacity() ? 2 * capacity() : static_cast<size_t>(1));
    }
    new(end()) T(copy);
    ++size_;
}

template<typename T>
void vector<T>::pop_back() noexcept {
    assert(size() > 0);
    data_[--size_].~T();
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(vector::iterator pos, T const &value) {
    return insert(static_cast<T const *>(pos), value);
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, T const &value) {
    assert(pos >= begin() && pos <= end());
    ptrdiff_t insertion_point = pos - begin();
    push_back(value);
    for (ptrdiff_t curr = size() - 1; curr - 1 >= insertion_point; --curr) {
        std::swap(data_[curr - 1], data_[curr]);
    }
    return data() + (pos - data());
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::iterator pos) {
    return erase(static_cast<T const *>(pos));
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator pos) {
    return erase(pos, pos + 1);
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::iterator first, vector::iterator last) {
    return erase(static_cast<T const *>(first), static_cast<T const *>(last));
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator first, vector::const_iterator last) {
    assert(first >= begin() && first <= last && last <= end() && last - first <= size());
    const ptrdiff_t n_deletions = last - first;
    for (ptrdiff_t curr = last - data(); curr < size(); ++curr) {
        std::swap(data_[curr - n_deletions], data_[curr]);
    }
    for (ptrdiff_t times = 0; times < n_deletions; ++times) {
        pop_back();
    }
    return data() + (first - data());
}

#define MY_VECTOR_MY_VECTOR_H

#endif //MY_VECTOR_MY_VECTOR_H