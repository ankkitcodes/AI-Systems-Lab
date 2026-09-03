#include "ExecutionEngine.h"

#include <stdexcept>
#include <unordered_map>
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

            for (float value : input_values)
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

Tensor ExecutionEngine::execute(
    const Model& model,
    const Tensor& input) const
{
    std::unordered_map<std::string, Tensor> tensors;

    if (input.name() != model.input_name())
    {
        throw std::runtime_error(
            "Input tensor name does not match model input"
        );
    }

    tensors.insert_or_assign(input.name(), input);

    for (const Node& node : model.nodes())
    {
        auto input_it = tensors.find(node.input_name());

        if (input_it == tensors.end())
        {
            throw std::runtime_error(
                "Input tensor not found: " + node.input_name()
            );
        }

        Tensor output = execute(node, input_it->second);

        tensors.insert_or_assign(node.output_name(), output);
    }

    auto output_it = tensors.find(model.output_name());

    if (output_it == tensors.end())
    {
        throw std::runtime_error(
            "Output tensor not found: " + model.output_name()
        );
    }

    return output_it->second;
}
