#include "ExecutionPlanner.h"

#include <stdexcept>
#include <unordered_set>

std::vector<Node> ExecutionPlanner::create_plan(const std::vector<Node>& nodes, const std::vector<std::string>& input_names) const
{
    std::vector<Node> sorted_nodes;

    std::unordered_set<std::string> available_tensors;

    for (const std::string& input_name : input_names)
    {
        available_tensors.insert(input_name);
    }

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

            bool inputs_available = true;

            for (const std::string& input_name : node.input_names())
            {
                if (available_tensors.find(input_name) ==  available_tensors.end())
                {
                    inputs_available = false;
                    break;
                }
            }

            if (!inputs_available)
            {
                continue;
            }
            sorted_nodes.push_back(node);
            executed.insert(i);

            for (const std::string& output_name : node.output_names())
            {
                available_tensors.insert(output_name);
            }
            progress = true;
        }
        if (!progress)
            {
                throw std::runtime_error(
                    "Could not determine graph execution order: "
                    "graph contains a missing dependency or cycle"
                );
            }
    }
    return sorted_nodes;
}