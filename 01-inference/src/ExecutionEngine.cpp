#include "ExecutionEngine.h"

#include <stdexcept>
#include <unordered_map>
#include <vector>


Tensor ExecutionEngine::execute(
    const Node& node,
    const std::vector<Tensor>& inputs) const
{
    if (inputs.empty())
    {
        throw std::runtime_error(
            "Operation received no inputs"
        );
    }

    switch (node.operation())
    {
        case OperationType::MULTIPLY:
        {
            const Tensor& input = inputs[0];

            std::vector<float> output_values;
            output_values.reserve(input.size());

            for (float value : input.values())
            {
                output_values.push_back(
                    value * node.parameter()
                );
            }

            return Tensor(output_values);
        }

        case OperationType::RELU:
        {
            const Tensor& input = inputs[0];

            std::vector<float> output_values;
            output_values.reserve(input.size());

            for (float value : input.values())
            {
                output_values.push_back(
                    value > 0.0f ? value : 0.0f
                );
            }

            return Tensor(output_values);
        }

        case OperationType::ADD:
        {
            if (inputs.size() != 2)
            {
                throw std::runtime_error(
                    "ADD expects exactly two inputs"
                );
            }

            const Tensor& first = inputs[0];
            const Tensor& second = inputs[1];

            if (first.size() != second.size())
            {
                throw std::runtime_error(
                    "ADD inputs must have the same size"
                );
            }

            std::vector<float> output_values;
            output_values.reserve(first.size());

            for (std::size_t i = 0; i < first.size(); ++i)
            {
                output_values.push_back(
                    first.values()[i] +
                    second.values()[i]
                );
            }

            return Tensor(output_values);
        }

        default:
        {
            throw std::runtime_error(
                "Unsupported operation"
            );
        }
    }
}


Tensor ExecutionEngine::execute(
    const Model& model,
    const std::vector<Tensor>& inputs) const
{
    if (inputs.size() != model.input_names().size())
    {
        throw std::runtime_error(
            "Number of input tensors does not match model inputs"
        );
    }

    std::unordered_map<std::string, Tensor> tensors;

    // Register all model input tensors.
    for (const Tensor& input : inputs)
    {
        if (input.name().empty())
        {
            throw std::runtime_error(
                "Input tensor must have a name"
            );
        }

        if (tensors.find(input.name()) != tensors.end())
        {
            throw std::runtime_error(
                "Duplicate input tensor: " + input.name()
            );
        }

        tensors.insert_or_assign(
            input.name(),
            input
        );
    }

    // Make sure every input required by the model was provided.
    for (const std::string& input_name : model.input_names())
    {
        if (tensors.find(input_name) == tensors.end())
        {
            throw std::runtime_error(
                "Required model input not provided: " +
                input_name
            );
        }
    }

    // Determine the order in which nodes should execute.
    const std::vector<Node> execution_order =
        planner_.create_plan(
            model.nodes(),
            model.input_names()
        );

    // Execute nodes in dependency order.
    for (const Node& node : execution_order)
    {
        if (node.output_names().size() != 1)
        {
            throw std::runtime_error(
                "Current execution engine expects exactly one output"
            );
        }

        std::vector<Tensor> node_inputs;

        for (const std::string& input_name : node.input_names())
        {
            auto input_it = tensors.find(input_name);

            if (input_it == tensors.end())
            {
                throw std::runtime_error(
                    "Input tensor not found: " + input_name
                );
            }

            node_inputs.push_back(
                input_it->second
            );
        }

        const std::string& output_name =
            node.output_names()[0];

        Tensor output = execute(
            node,
            node_inputs
        );

        tensors.insert_or_assign(
            output_name,
            output
        );
    }

    // Find the final graph output.
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