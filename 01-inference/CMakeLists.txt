cmake_minimum_required(VERSION 3.16)

project(MiniAIRuntime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RUNTIME_SOURCES
    src/Tensor.cpp
    src/Node.cpp
    src/Model.cpp
    src/ExecutionEngine.cpp
    src/ExecutionPlanner.cpp
    src/Runtime.cpp
    src/ModelLoader.cpp
)

add_executable(mini_ai_runtime
    src/main.cpp
    ${RUNTIME_SOURCES}
)

target_include_directories(mini_ai_runtime PRIVATE include)


add_executable(test_runtime
    tests/test_runtime.cpp
    ${RUNTIME_SOURCES}
)

target_include_directories(test_runtime PRIVATE include)

# Avoid DLL conflicts when conda/anaconda is on PATH (common on Windows).
if(MINGW)
    target_link_options(mini_ai_runtime PRIVATE
        -static-libgcc
        -static-libstdc++
    )

    target_link_options(test_runtime PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

enable_testing()

add_test(
    NAME runtime_test
    COMMAND test_runtime
)