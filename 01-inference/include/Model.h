#pragma once
#include "Node.h"
#include <vector>

class Model
{
public:
    void add_node(const Node& node);
    const std::vector<Node>& nodes() const;

private:
    std::vector<Node> nodes_;
};