#pragma once
#include "Node.h"
#include <vector>
#include <string>

class Model
{
public:
    void add_node(const Node& node);

    void set_input(const std::string& input_name);
    void set_output(const std::string& output_name);

    const std::vector<Node>& nodes() const;

    const std::string& input_name() const;
    const std::string& output_name() const;

private:
    std::vector<Node> nodes_;
    std::string input_name_;
    std::string output_name_;
};