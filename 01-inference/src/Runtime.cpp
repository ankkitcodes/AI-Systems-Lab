#include "Runtime.h"

Tensor Runtime::run(const Model& model, const std::vector<Tensor>& inputs) const
{
    return execution_engine_.execute(model, inputs);
}