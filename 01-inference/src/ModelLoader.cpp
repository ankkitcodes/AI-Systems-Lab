#include "ModelLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

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

            model.set_input(input_name);
        }
        else if (keyword == "OUTPUT")
        {
            std::string output_name;

            stream >> output_name;

            model.set_output(output_name);
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

                model.add_node(
                    Node(
                        OperationType::MULTIPLY,
                        input_name,
                        output_name,
                        parameter
                    )
                );
            }
            else if (operation == "ADD")
            {
                float parameter;

                stream >> parameter;

                model.add_node(
                    Node(
                        OperationType::ADD,
                        input_name,
                        output_name,
                        parameter
                    )
                );
            }
            else if (operation == "RELU")
            {
                model.add_node(
                    Node(
                        OperationType::RELU,
                        input_name,
                        output_name
                    )
                );
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

    return model;
}