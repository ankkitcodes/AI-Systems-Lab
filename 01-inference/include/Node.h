#pragma once
#include <cstddef>

enum class OperationType
{
    ADD,
    MULTIPLY,
    RELU
};

class Node
{
public:
    Node(OperationType operation, float parameter = 0.0f);
    OperationType operation() const;
    float parameter() const;

private:
    OperationType operation_;
    float parameter_;
};
