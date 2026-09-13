#pragma once

#include "ExecutionPlanner.h"
#include "Model.h" 
#include "Node.h"
#include "Tensor.h"

#include <vector>

class ExecutionEngine
{
public:
    Tensor execute(const Node& node, const std::vector<Tensor>& inputs) const;

    Tensor execute(const Model& model, const std::vector<Tensor>& inputs) const;

private:
    ExecutionPlanner planner_;
};

