#include "Model.h"

void Model::add_node(const Node& node)
{
    nodes_.push_back(node);
}

void Model::add_input(const std::string& input_name)
{
    input_names_.push_back(input_name);
}

void Model::set_output(const std::string& output_name)
{
    output_name_ = output_name;
}

const std::vector<Node>& Model::nodes() const
{
    return nodes_;
}

const std::vector<std::string>& Model::input_names() const
{
    return input_names_;
}

const std::string& Model::output_name() const
{
    return output_name_;
}
