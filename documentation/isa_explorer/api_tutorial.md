# Tutorial: Exploring AMD ISA Instructions Using `IsaExplorer` API

## Introduction

This tutorial demonstrates how to use the `amdisa::explorer::Spec` API to load an AMD GPU ISA specification file and query information about instructions. The program initializes the ISA specification, retrieves details about a specific instruction (`V_MOV_B32`), and randomly selects another instruction to display. By following this guide, you'll learn to leverage the `Spec` API for exploring AMD ISA.

---

## Prerequisites

Before running the code, ensure you have:

1. **AMD ISA Specification File:** Obtain the XML specification file for the target GPU architecture.
2. **C++ Development Environment:** A compiler supporting C++11 or later and the required libraries for the `amdisa` namespace.
3. **Utility Functions:** The `print_instruction` function must be defined to display instruction details. You can create it to suit your display needs.

---

## Walkthrough

### 1. Initializing the `Spec` Object

The `Spec` object from the `amdisa::explorer` namespace is used to load and interact with the ISA specification.

```cpp
amdisa::explorer::Spec explorer;
std::string            err_message;
bool                   is_init = explorer.Init(kPathToSpec, err_message);

if (!is_init)
{
    std::cerr << err_message << std::endl;
    return -1;
}
```

- The `Init` function reads the XML specification file and checks for errors.
- If initialization fails, an error message is printed, and the program exits.

### 2. Looking Up a Specific Instruction

The `GetInstructions` method retrieves a map of all instructions available in the specification. The program queries for the `V_MOV_B32` instruction.

```cpp
auto v_mov_b32 = explorer.GetInstructions().at("V_MOV_B32");
print_instruction(v_mov_b32);
```

The instruction data is passed to the `print_instruction` function, which should format and display the information.

### 3. Randomly Selecting  an Instruction

Alternatively, you can iterate over the map:

```cpp
std::random_device                 rd;
std::mt19937                       gen(rd());
std::uniform_int_distribution<int> dist(0, explorer.GetInstructions().size() - 1);

auto it = explorer.GetInstructions().begin();
std::advance(it, dist(gen));
print_instruction(it->second);
```

- **Random Device and Generator:** Creates a random number generator seeded with a hardware-based random device.
- **Uniform Distribution:** Ensures the random number falls within the valid range of instruction indices.
- **Iterator Advancement:** Randomly selects an instruction by advancing the iterator to the generated index.

The selected instruction is printed using the `print_instruction` function.

As an exmple, you can use the following print function:

```cpp
static void print_instruction(const amdisa::explorer::Instruction& instruction)
{
    std::cout << "Name: " << instruction.Name() << std::endl;
    std::cout << "\tDescription: " << instruction.Description() << std::endl;
    std::cout << "\tIsBranch ?: " << instruction.IsBranch() << std::endl;
    std::cout << "\tIsConditionalBranch ?: " << instruction.IsConditionalBranch() << std::endl;
    std::cout << "\tIsIndirectBranch ?: " << instruction.IsIndirectBranch() << std::endl;
    std::cout << "\tFunctionalGroup: " << instruction.FuncGroup()->Name() << std::endl;
    std::cout << "\t\t Description: " << instruction.FuncGroup()->Description() << std::endl << std::endl;
    std::cout << "\tFunctionalSubgroup: " << instruction.FuncSubgroup()->Name() << std::endl;
}
```