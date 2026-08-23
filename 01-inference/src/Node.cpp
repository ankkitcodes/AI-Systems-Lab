#include "Node.h"

Node::Node(OperationType operation, float parameter) 
    : operation_(operation), parameter_(parameter) 
    {}
OperationType Node::operation() const{
    return operation_;
}

float Node::parameter() const{
    return parameter_;
}