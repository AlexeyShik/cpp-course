#include "shared_vector.h"
#include <utility>
#include <vector>
#include <cstdint>

shared_vector::shared_vector(const size_t size, const uint32_t val) : data(size, val), ref_count(1) {}

shared_vector::shared_vector(std::vector<uint32_t> other_data) : data(std::move(other_data)), ref_count(1) {}

shared_vector::shared_vector(const shared_vector &other) : data(other.data), ref_count(1) {}
