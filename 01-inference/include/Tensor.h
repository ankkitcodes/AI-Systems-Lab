#pragma once

#include <vector>
#include <cstddef>
#include <string>

class Tensor {
public:
    // Create a tensor from a list of float values
    Tensor(const std::vector<float>& values);
    Tensor(const std::string& name, const std::vector<float>& values);

    const std::string& name() const;
    // Return the number of elements
    std::size_t size() const;
    // Return the tensor data
    const std::vector<float>& values() const;

private:
    std::string name_;
    std::vector<float> values_;
};