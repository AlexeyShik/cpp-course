#include <vector>
#include <cstdint>

#ifndef BIGINT_shared_vector_H
#define BIGINT_shared_vector_H

struct shared_vector {
    ///  @variables
public:
    std::vector<uint32_t> data;  ///  shared data
    size_t ref_count;  ///  number of references on shared data

    ///  @methods
public:
    explicit shared_vector(size_t size, uint32_t val);

    explicit shared_vector(std::vector<uint32_t> other_data);

    shared_vector(const shared_vector &other);
};

#endif //BIGINT_shared_vector_H
