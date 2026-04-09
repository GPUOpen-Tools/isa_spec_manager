
#include <iostream>
#include <random>

#include "amdisa/isa_explorer.h"

// Constants.
// Error constant string.
static const char* kStrErrorInvalidArgument = "Error: Requires XML specification path.";

static void print_instruction(const amdisa::explorer::Instruction& instruction)
{
    std::cout << "Name: " << instruction.Name() << std::endl;
    std::cout << "\tDescription: " << instruction.Description() << std::endl;
    std::cout << "\tIsBranch ?: " << instruction.IsBranch() << std::endl;
    std::cout << "\tIsConditionalBranch ?: " << instruction.IsConditionalBranch() << std::endl;
    std::cout << "\tIsIndirectBranch ?: " << instruction.IsIndirectBranch() << std::endl;
    std::cout << "\tFunctionalGroup: " << instruction.FuncGroup()->Name() << std::endl;
    std::cout << "\t\t Description: " << instruction.FuncGroup()->Description() << std::endl << std::endl;
    std::cout << "\tFunctionalSubgroup: ";
    for (const auto& subgroup : instruction.FuncSubgroups())
    {
        std::cout << subgroup.Name() << "; ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    // Check for valid input argument.
    if (argc != 2)
    {
        std::cerr << kStrErrorInvalidArgument << std::endl;
        return -1;
    }

    amdisa::explorer::Spec explorer;

    // Check API version.
    std::cout << "IsaExplorer version: " << explorer.GetApiVersion() << std::endl;

    const std::string kPathToSpec = argv[1];
    std::string            err_message;
    bool                   is_init = explorer.Init(kPathToSpec, err_message);

    // Check for successful spec initialization.
    if (!is_init)
    {
        std::cerr << err_message << std::endl;
        return -1;
    }

    // List all Functional Groups and their corresponding list of instructions.
    const auto& functional_groups = explorer.GetFunctionalGroups();
    for (const auto& fg : functional_groups)
    {
        std::cout << fg.first << ":" << std::endl;
        for (const auto& instr : fg.second.Instructions())
        {
            std::cout << "\t" << instr->Name() << std::endl;
        }
    }

    std::cout << std::endl << "-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;

    // Lookup an instruction by name.
    auto v_mov_b32 = explorer.GetInstructions().at("V_MOV_B32");
    print_instruction(v_mov_b32);

    // Choose a random instruction and print it.
    std::random_device                 rd;
    std::mt19937                       gen(rd());
    std::uniform_int_distribution<int> dist(0, explorer.GetInstructions().size() - 1);

    auto it = explorer.GetInstructions().begin();
    std::advance(it, dist(gen));
    print_instruction(it->second);

    return 0;
}
