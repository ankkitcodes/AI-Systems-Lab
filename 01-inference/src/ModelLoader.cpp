#include "ModelLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
Model ModelLoader::load(const std::string& file_path) const
{
    std::ifstream file(file_path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open model file: " + file_path
        );
    }

    Model model;

    std::string line;

    bool has_input = false;
    bool has_output = false;
    bool has_node = false;

    std::unordered_set<std::string> available_tensors;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream stream(line);

        std::string keyword;
        stream >> keyword;

        if (keyword == "INPUT")
        {
            std::string input_name;

            stream >> input_name;
            if (input_name.empty())
            {
                throw std::runtime_error("INPUT requires a tensor name");
            }
            model.set_input(input_name);
            has_input = true;
            available_tensors.insert(input_name);
        }

        else if (keyword == "OUTPUT")
        {
            std::string output_name;

            stream >> output_name;
            if (output_name.empty())
            {
                throw std::runtime_error("OUTPUT requires a tensor name");
            }
            model.set_output(output_name);
            has_output = true;
        }
        else if (keyword == "NODE")
        {
            std::string operation;
            std::string input_name;
            std::string output_name;

            stream >> operation;
            stream >> input_name;
            stream >> output_name;

            if (operation == "MULTIPLY")
            {
                float parameter;

                stream >> parameter;
                
                if (available_tensors.find(output_name) != available_tensors.end())
                {
                    throw std::runtime_error("Tensor already exists: " + output_name);
                }
                model.add_node(
                    Node(
                        OperationType::MULTIPLY,
                        input_name,
                        output_name,
                        parameter
                    )
                );
                has_node = true;
                available_tensors.insert(output_name);
            }
            else if (operation == "ADD")
            {
                float parameter;

                stream >> parameter;

                if (available_tensors.find(output_name) != available_tensors.end())
                {
                    throw std::runtime_error("Tensor already exists: " + output_name);
                }

                model.add_node(
                    Node(
                        OperationType::ADD,
                        input_name,
                        output_name,
                        parameter
                    )
                );
                has_node = true;
                available_tensors.insert(output_name);
            }
            else if (operation == "RELU")
            {
                
                if (available_tensors.find(output_name) != available_tensors.end())
                {
                    throw std::runtime_error("Tensor already exists: " + output_name);
                }
                model.add_node(
                    Node(
                        OperationType::RELU,
                        input_name,
                        output_name
                    )
                );
                available_tensors.insert(output_name);
                has_node = true;
            }
            else
            {
                throw std::runtime_error(
                    "Unsupported operation: " + operation
                );
            }
        }
        else
        {
            throw std::runtime_error(
                "Unknown model directive: " + keyword
            );
        }
    }

    if (!has_input)
    {
        throw std::runtime_error("Model is missing INPUT");
    }
    if (!has_output)
    {
        throw std::runtime_error("Model is missing OUTPUT");
    }
    if (!has_node)
    {
        throw std::runtime_error("Model contains no nodes");
    }
    if (available_tensors.find(model.output_name())==available_tensors.end())
    {
        throw std::runtime_error("Model output tensor not produced: " + model.output_name());
    }
    return model;
}