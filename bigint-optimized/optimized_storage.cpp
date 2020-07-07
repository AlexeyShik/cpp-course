#include "optimized_storage.h"
#include <cassert>

optimized_storage::optimized_storage(size_t size, uint32_t val) {
    if (size > MAX_STATIC_SIZE) {
        ptr = new shared_vector(size, val);
    } else {
        std::fill(static_data.begin(), static_data.begin() + size, val);
        std::fill(static_data.begin() + size, static_data.end(), 0);
    }
    set_size(size);
}

optimized_storage::optimized_storage(const optimized_storage &other) {
    if (other.small) {
        static_data = other.static_data;
    } else if (other.size_ <= MAX_STATIC_SIZE) {
        fill_static_from_other_dynamic(other);
    } else {
        ptr = new shared_vector(*other.ptr);
    }
    set_size(other.size_);
}

optimized_storage::~optimized_storage() {
    if (!small && ptr->ref_count == 1) {
        delete ptr;
    } else if (!small) {
        --ptr->ref_count;
    }
}

optimized_storage &optimized_storage::operator=(const optimized_storage &other) {
    if (this != &other) {
        this->~optimized_storage();
        if (other.small) {
            static_data = other.static_data;
        } else if (other.size_ <= MAX_STATIC_SIZE) {
            fill_static_from_other_dynamic(other);
        } else {
            ptr = other.ptr;
            ++ptr->ref_count;
        }
        set_size(other.size_);
    }
    return *this;
}

const uint32_t &optimized_storage::operator[](size_t i) const {
    return small ? static_data[i] : ptr->data[i];
}

uint32_t &optimized_storage::operator[](size_t i) {
    return small ? static_data[i] : ptr->data[i];
}

size_t optimized_storage::size() const {
    return size_;
}

bool operator==(const optimized_storage &a, const optimized_storage &b) {
    if (a.size_ != b.size_) {
        return false;
    }
    for (size_t i = 0; i < a.size_; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

uint32_t optimized_storage::back() const {
    return small ? static_data[size_ - 1] : ptr->data[size_ - 1];
}

void optimized_storage::pop_back() {
    if (small) {
        static_data[--size_] = 0;
    } else {
        ptr->data.pop_back();
        --size_;
    }
}

void optimized_storage::push_back(uint32_t x) {
    if (small && size_ + 1 <= MAX_STATIC_SIZE) {
        static_data[size_] = x;
    } else {
        if (small) {  ///  converts from static storage to dynamic, after insertions size will be > MAX_STATIC_SIZE
            std::vector<uint32_t> tmp(static_data.begin(), static_data.begin() + size_);
            static_data.~array<uint32_t, MAX_STATIC_SIZE>();
            ptr = new shared_vector(tmp);
            small = false;
        }
        ptr->data.push_back(x);
    }
    ++size_;
}

void optimized_storage::set_size(const size_t new_size) {
    size_ = new_size;
    small = size_ <= MAX_STATIC_SIZE;
}

void optimized_storage::fill_static_from_other_dynamic(const optimized_storage &other) {
    assert(other.size_ <= MAX_STATIC_SIZE);
    std::copy(other.ptr->data.begin(), other.ptr->data.end(), static_data.begin());
    std::fill(static_data.begin() + other.size_, static_data.end(), 0);
}
