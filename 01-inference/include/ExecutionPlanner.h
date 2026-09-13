#pragma once
#include "Node.h"

#include <string>
#include <vector>

class ExecutionPlanner
{
public:
    std::vector<Node> create_plan(const std::vector<Node>& nodes, const std::vector<std::string>& input_names) const;
};
