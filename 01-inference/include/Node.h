#pragma once
#include <cstddef>
#include <string>

enum class OperationType
{
    ADD,
    MULTIPLY,
    RELU
};

class Node
{
public:
    Node(
        OperationType operation, 
        const std::string& input_name,
        const std::string& output_name,
        float parameter = 0.0f
    );

    OperationType operation() const;
    const std::string& input_name() const;
    const std::string& output_name() const;
    float parameter() const;

private:
    OperationType operation_;
    std::string input_name_;
    std::string output_name_;
    float parameter_;
};
