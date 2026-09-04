#pragma once
#include "Model.h"
#include <string>

class ModelLoader
{
public:
    Model load(const std::string& file_path) const;
};
