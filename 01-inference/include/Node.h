#pragma once
#include <vector>
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
        const std::vector<std::string>& input_names,
        const std::vector<std::string>& output_names,
        float parameter = 0.0f
    );

    OperationType operation() const;
    const std::vector<std::string>& input_names() const;
    const std::vector<std::string>& output_names() const;
    float parameter() const;

private:
    OperationType operation_;

    std::vector<std::string> input_names_;
    std::vector<std::string> output_names_;
    float parameter_;
};
