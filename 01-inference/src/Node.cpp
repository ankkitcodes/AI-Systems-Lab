#include "Node.h"

Node::Node(OperationType operation, 
    const std::string& input_name,
    const std::string& output_name,
    float parameter) 
    : operation_(operation), 
    input_name_(input_name),
    output_name_(output_name),
    parameter_(parameter) 
    {}
OperationType Node::operation() const{
    return operation_;
}

const std::string& Node::input_name() const{
    return input_name_;
}
const std::string& Node::output_name() const{
    return output_name_;
}

float Node::parameter() const{
    return parameter_;
}