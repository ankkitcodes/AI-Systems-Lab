#include "Tensor.h"

Tensor::Tensor(const std::vector<float>& values)
    : values_(values)

{
}
Tensor::Tensor(const std::string& name, const std::vector<float>& values)
    : name_(name), values_(values)
{
}
const std::string& Tensor::name() const
{
    return name_;
}


std::size_t Tensor::size() const
{
    return values_.size();
}

const std::vector<float>& Tensor::values() const
{
    return values_;
}