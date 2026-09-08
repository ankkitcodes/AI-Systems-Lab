#include "ExecutionEngine.h"

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<Node> topological_sort(
    const std::vector<Node>& nodes,
    const std::string& input_name)
{
    std::vector<Node> sorted_nodes;

    std::unordered_set<std::string> available_tensors;
    available_tensors.insert(input_name);

    std::unordered_set<std::size_t> executed;

    while (sorted_nodes.size() < nodes.size())
    {
        bool progress = false;

        for (std::size_t i = 0; i < nodes.size(); ++i)
        {
            if (executed.find(i) != executed.end())
            {
                continue;
            }

            const Node& node = nodes[i];

            if (available_tensors.find(node.input_name())
                == available_tensors.end())
            {
                continue;
            }

            sorted_nodes.push_back(node);
            executed.insert(i);

            available_tensors.insert(node.output_name());

            progress = true;
        }

        if (!progress)
        {
            throw std::runtime_error(
                "Could not determine graph execution order"
            );
        }
    }

    return sorted_nodes;
}

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
                output_values.push_back(
                    value * node.parameter()
                );
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
                output_values.push_back(
                    value + node.parameter()
                );
            }

            break;
        }

        default:
        {
            throw std::runtime_error(
                "Unsupported operation"
            );
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

    tensors.insert_or_assign(
        input.name(),
        input
    );

    const std::vector<Node> execution_order =
        topological_sort(
            model.nodes(),
            model.input_name()
        );

    for (const Node& node : execution_order)
    {
        auto input_it = tensors.find(node.input_name());

        if (input_it == tensors.end())
        {
            throw std::runtime_error("Input tensor not found: " + node.input_name());
        }

        Tensor output = execute(node,input_it->second);

        tensors.insert_or_assign(node.output_name(), output);
    }

    auto output_it = tensors.find(
        model.output_name()
    );

    if (output_it == tensors.end())
    {
        throw std::runtime_error(
            "Output tensor not found: " +
            model.output_name()
        );
    }

    return output_it->second;
}