#pragma once

#include "Model.h" 
#include "Node.h"
#include "Tensor.h"

class ExecutionEngine
{
public:
    Tensor execute(const Node& node, const Tensor& input) const;

    Tensor execute(const Model& model, const Tensor& input) const;
};

