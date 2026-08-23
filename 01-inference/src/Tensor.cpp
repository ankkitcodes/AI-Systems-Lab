#include "Tensor.h"

Tensor::Tensor(const std::vector<float>& values)
    : data_(values)

{
}
std::size_t Tensor::size() const
{
    return data_.size();
}

const std::vector<float>& Tensor::values() const
{
    return data_;
}