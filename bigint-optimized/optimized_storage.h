#include "shared_vector.h"
#include <cstdint>
#include <array>

#ifndef BIGINT_OPTIMIZED_STORAGE_H
#define BIGINT_OPTIMIZED_STORAGE_H

struct optimized_storage {
    /// @consts
private:
    static constexpr size_t MAX_STATIC_SIZE = sizeof(shared_vector *) / sizeof(uint32_t);  ///  optimal size

    ///  @variables
private:
    union {
        shared_vector *ptr;  ///  dynamic storage
        std::array<uint32_t, MAX_STATIC_SIZE> static_data;  /// static storage
    };

    size_t size_;  ///  number of "digits" in data

    bool small;  ///  true, if data stores in static storage

    ///  @methods
public:
    explicit optimized_storage(size_t size, uint32_t val);

    optimized_storage(const optimized_storage &other);

    ~optimized_storage();

    optimized_storage &operator=(const optimized_storage &other);

    const uint32_t &operator[](size_t i) const;

    uint32_t &operator[](size_t i);

    size_t size() const;

    friend bool operator==(const optimized_storage &a, const optimized_storage &b);

    uint32_t back() const;

    void pop_back();

    void push_back(uint32_t x);

private:
    void set_size(size_t new_size);  ///  updates size_ and small

    void fill_static_from_other_dynamic(const optimized_storage &other);  /// trying to avoid allocating dynamic memory

    void make_unshared();  ///  if *this shares data, then makes data unique

    void swap(optimized_storage &other);
};

bool operator==(const optimized_storage &a, const optimized_storage &b);

#endif //BIGINT_OPTIMIZED_STORAGE_H
