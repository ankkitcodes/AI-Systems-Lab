#include "Node.h"

Node::Node(
    OperationType operation,
    const std::vector<std::string>& input_names,
    const std::vector<std::string>& output_names,
    float parameter
)
    : operation_(operation),
      input_names_(input_names),
      output_names_(output_names),
      parameter_(parameter)
{
}

OperationType Node::operation() const
{
    return operation_;
}

const std::vector<std::string>& Node::input_names() const
{
    return input_names_;
}

const std::vector<std::string>& Node::output_names() const
{
    return output_names_;
}

float Node::parameter() const
{
    return parameter_;
}