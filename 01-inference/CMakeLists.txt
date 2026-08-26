cmake_minimum_required(VERSION 3.16)

project(MiniAIRuntime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mini_ai_runtime
    src/main.cpp
    src/Tensor.cpp
    src/Node.cpp
    src/ExecutionEngine.cpp
    src/Model.cpp
    src/Runtime.cpp
)

target_include_directories(mini_ai_runtime PRIVATE include)

# Avoid DLL conflicts when conda/anaconda is on PATH (common on Windows).
if(MINGW)
    target_link_options(mini_ai_runtime PRIVATE -static-libgcc -static-libstdc++)
endif()