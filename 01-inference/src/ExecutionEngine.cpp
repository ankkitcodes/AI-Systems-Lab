#include "ExecutionEngine.h"

#include <stdexcept>
#include <vector>

Tensor ExecutionEngine::execute(
    const Node& node,
    const Tensor& input) const
{
    const auto& input_values = input.values();

    std::vector<float> output_values;

    switch (node.operation())
    {
        case OperationType::MULTIPLY:
        {
            output_values.reserve(input.size());

            for(float value : input_values)
            {
                output_values.push_back(value * node.parameter());
            }
            break;
        }

        case OperationType::RELU:
        {
            output_values.reserve(input.size());
            for (float value : input_values)
            {
                output_values.push_back(
                    value > 0.0f ? value : 0.0f
                );
            }
            break;
        }

        case OperationType::ADD:
        {
            output_values.reserve(input.size());
            for (float value : input_values)
            {
                output_values.push_back(value + node.parameter());
            }
            break;
        }
        default:
        {
            throw std::runtime_error("Unsupported operation");
        }
    }
    return Tensor(output_values);
    
}