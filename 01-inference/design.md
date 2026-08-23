# Mini AI Runtime - Design Document

## 1. Problem Statement

A trained AI model stored in a model format cannot be executed directly as an application-level prediction. Before inference can be performed, software must load the model, understand its computation graph, prepare the input tensors, execute the required operations, and return the output.

This project aims to design and build a simplified AI Runtime that performs these responsibilities.

---

## 2. Goal

The goal of this project is to build a simplified AI Runtime capable of executing a trained neural network model.

The runtime will:

- Load a model.
- Represent and understand the model's computation graph.
- Accept tensor input.
- Execute supported operations.
- Produce an output tensor.

The objective is not to compete with production runtimes such as TensorRT or ONNX Runtime, but to understand the engineering principles behind their design.

---



## 3. Scope

The first version of the Mini AI Runtime will focus on the basic lifecycle of model inference.

The runtime will:

- Load a simplified model representation.
- Represent a computation graph.
- Accept tensor input.
- Support a limited number of operations.
- Execute operations in the correct order.
- Produce an output tensor.

The first version will not include:

- GPU execution.
- CUDA kernels.
- Advanced graph optimization.
- Dynamic batching.
- Distributed inference.
- Support for all ONNX operators.
- Performance optimization comparable to TensorRT or ONNX Runtime.

These capabilities may be explored in later stages of the AI Systems Lab.

---



## 4. Inputs

The runtime will receive:

### Model

Initially, the project will use a simplified internal model representation rather than immediately supporting full ONNX parsing.

The model will describe:

- Operations or layers.
- The order in which they execute.
- Inputs and outputs.
- Parameters required by operations.



### Input Tensor

The runtime will receive numerical tensor data as input.

For example:

[1.0, 2.0, 3.0]

The runtime will pass tensors through the computation graph.

---



## 5. Outputs

The primary output of the first version will be an output tensor.

For example:

Input:

[1.0, 2.0, 3.0]

Output:

[2.0, 4.0, 6.0]

Later versions may convert output tensors into higher-level predictions such as:

- Classification labels.
- Confidence scores.
- Detection results.

---



## 6. High-Level Architecture

The system will consist of the following conceptual components:

User
  |
  v
Runtime
  |
  +--> Model Loader
  |
  +--> Graph Representation
  |
  +--> Execution Engine
  |
  +--> Tensor Management
  |
  v
Output Tensor

The Runtime acts as the main coordinator of the system.

---



## 7. Component Responsibilities



### Runtime

Coordinates the inference process.

Responsibilities:

- Accept the model and input.
- Coordinate the different components.
- Trigger execution.
- Return the final output.



### Model Loader

Responsible for loading the model representation.

Responsibilities:

- Read the model from disk.
- Validate basic model information.
- Create the internal representation of the model.



### Graph Representation

Represents the model as a computation graph.

For example:

Input
  |
  v
Multiply
  |
  v
ReLU
  |
  v
Output

The graph defines what operations exist and how data flows between them.

### Tensor

Represents numerical data flowing through the runtime.

Examples include:

- Input tensors.
- Intermediate tensors.
- Output tensors.



### Execution Engine

Responsible for executing the computation graph.

Responsibilities:

- Determine the execution order.
- Execute supported operations.
- Pass tensors between operations.
- Produce the final output tensor.

---



## 8. Data Flow

The simplified inference flow will be:

Model File
    |
    v
Model Loader
    |
    v
Computation Graph
    |
    +------------------+
                       |
Input Tensor ----------+
                       |
                       v
                Execution Engine
                       |
                       v
                 Output Tensor

The model is loaded once and can later be reused for multiple inference requests.

---



## 9. Initial Supported Operations

The first version will support only a small number of operations.

Possible operations include:

- Add
- Multiply
- ReLU

For example:

Input

[1, -2, 3]

```
    |
    v
```

Multiply by 2

[2, -4, 6]

```
    |
    v
```

ReLU

[2, 0, 6]

This allows us to focus on runtime architecture before dealing with the complexity of real neural network operators.

---



## 10. Initial Class Design

The first version may contain the following classes:

- Tensor
- Model
- Node
- ModelLoader
- ExecutionEngine
- Runtime

Conceptually:

Runtime
   |
   +--> ModelLoader
   |
   +--> Model
           |
           +--> Graph
                   |
                   +--> Node
   |
   +--> ExecutionEngine
           |
           +--> Tensor

Each class should have a clear responsibility.

---



## 11. Development Plan

The project will be developed incrementally.

### Phase 1: Core Data Structures

Implement:

- Tensor
- Node
- Model



### Phase 2: Model Representation

Create a simplified model format and load it into memory.

### Phase 3: Execution Engine

Implement execution for simple operations such as:

- Add
- Multiply
- ReLU



### Phase 4: Runtime Interface

Create a Runtime class that coordinates model loading and execution.

### Phase 5: Testing

Add unit tests for:

- Tensor operations.
- Individual nodes.
- Graph execution.
- End-to-end inference.



### Phase 6: Benchmarking

Measure:

- Execution time.
- Memory usage.
- Performance as graph size increases.

---



## 12. Future Extensions

Possible future improvements include:

- Real ONNX parsing.
- Additional operators.
- Graph optimization.
- Operator fusion.
- Memory planning.
- CPU parallelism.
- CUDA execution.
- TensorRT comparison.

These extensions will help connect this simplified runtime to real AI inference systems.