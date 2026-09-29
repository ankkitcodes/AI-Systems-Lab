#include "Tensor.h"

#include <stdexcept>

Tensor::Tensor(
    const std::vector<float>& values
)
    : values_(values)
{
}

Tensor::Tensor(
    const std::string& name,
    const std::vector<float>& values
)
    : name_(name),
      values_(values)
{
}

Tensor::Tensor(
    const std::vector<float>& values,
    const std::vector<std::size_t>& shape
)
    : values_(values),
      shape_(shape)
{
    validate_shape();
}

Tensor::Tensor(
    const std::string& name,
    const std::vector<float>& values,
    const std::vector<std::size_t>& shape
)
    : name_(name),
      values_(values),
      shape_(shape)
{
    validate_shape();
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

const std::vector<std::size_t>& Tensor::shape() const
{
    return shape_;
}

void Tensor::validate_shape() const
{
    if (shape_.empty())
    {
        throw std::runtime_error(
            "Tensor shape must contain at least one dimension"
        );
    }

    std::size_t expected_size = 1;

    for (std::size_t dimension : shape_)
    {
        if (dimension == 0)
        {
            throw std::runtime_error(
                "Tensor dimensions must be greater than zero"
            );
        }

        expected_size *= dimension;
    }

    if (expected_size != values_.size())
    {
        throw std::runtime_error(
            "Tensor shape does not match number of values"
        );
    }
}