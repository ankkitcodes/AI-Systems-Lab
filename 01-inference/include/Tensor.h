#pragma once

#include <vector>
#include <cstddef>
#include <string>

class Tensor {
public:
    // Create a tensor from a list of float values
    Tensor(const std::vector<float>& values);
    Tensor(const std::string& name, const std::vector<float>& values);

    // Create a tensor with explicit shape
    Tensor(const std::vector<float>& values, const std::vector<std::size_t>& shape);
    Tensor(const std::string& name, const std::vector<float>& values, const std::vector<std::size_t>& shape);
    
    const std::string& name() const;
    // Return the number of elements
    std::size_t size() const;
    // Return the tensor data
    const std::vector<float>& values() const;
    // Return the tensor shape
    const std::vector<std::size_t>& shape() const;

private:
    void validate_shape() const;
    
    std::string name_;
    std::vector<float> values_;
    std::vector<std::size_t> shape_;
};