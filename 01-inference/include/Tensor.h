#pragma once

#include <vector>
#include <cstddef>

class Tensor {
public:
    // Create a tensor from a list of float values
    Tensor(const std::vector<float>& values);
    
    // Return the number of elements
    std::size_t size() const;

    // Return the tensor data
    const std::vector<float>& values() const;

private:
    std::vector<float> data_;
};