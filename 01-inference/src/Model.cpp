#include "Model.h"
void Model::add_node(const Node& node)
{
    nodes_.push_back(node);
}

const std::vector<Node>& Model::nodes() const
{
    return nodes_;
}