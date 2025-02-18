# `IsaExplorer` Documentation

`IsaExplorer` is a set of utility classes for exploring the AMD GPU ISA specification contents. This API has the ability to iterate over a large subset of the specification contents

The `Spec` class is a top level user facing interface allowing access to different pieces of information about the specification. It allows users to load an XML-based ISA specification file and access detailed information about the architecture, data formats, instructions, and operand types defined in the ISA.

## Public Member Functions

### `bool Init(const std::string& input_xml_file_path, std::string& err_message) noexcept`

Reads an XML file containing the ISA specification and populates the internal data structures.

#### Parameters

| Parameter Name        | Type                 | Description                                                                    |
| --------------------- | -------------------- | ------------------------------------------------------------------------------ |
| `input_xml_file_path` | `const std::string&` | Path to the XML file containing the ISA specification.                         |
| `err_message`         | `std::string&`       | Reference to a string that will hold an error message if initialization fails. |

#### Return Value

Returns `true` if the XML file was successfully parsed and internal structures were initialized. Returns `false` otherwise.

#### Example

```c++
#include "amdisa/isa_explorer.h"

int main() {
    Spec isa_spec;
    std::string err_message;
    if (!isa_spec.Init("path/to/isa_spec.xml", err_message)) {
        std::cerr << "Initialization failed: " << err_message << std::endl;
        return -1;
    }
    return 0;
}
```

---

### `const Architecture& GetArchitecture() const noexcept`

Retrieves the architecture object associated with the ISA.

#### Return Value

Returns a `const` reference to the `Architecture` object.

#### Example

```c++
#include "amdisa/isa_explorer.h"

int main() {
    Spec isa_spec;
    std::string err_message;
    if (isa_spec.Init("path/to/isa_spec.xml", err_message)) {
        const auto& arch = isa_spec.GetArchitecture();
        // Use the architecture object...
    }
    return 0;
}
```

---

### `const std::map<std::string, DataFormat>& GetDataFormats() const noexcept`

Retrieves the data formats defined in the ISA.

#### Return Value

Returns a `const` reference to a map where the key is the name of the data format, and the value is the corresponding `DataFormat` object.

#### Example

```c++
#include "amdisa/isa_explorer.h"

int main() {
    Spec isa_spec;
    std::string err_message;
    if (isa_spec.Init("path/to/isa_spec.xml", err_message)) {
        const auto& data_formats = isa_spec.GetDataFormats();
        // Use the data_format object...
    }
    return 0;
}
```

---

### `const std::map<std::string, Instruction>& GetInstructions() const noexcept`

Retrieves the instructions defined in the ISA.

#### Return Value

Returns a `const` reference to a map where the key is the instruction name, and the value is the corresponding `Instruction` object.

#### Example

```c++
#include "amdisa/isa_explorer.h"

int main() {
    Spec isa_spec;
    std::string err_message;
    if (isa_spec.Init("path/to/isa_spec.xml", err_message)) {
        const auto& instructions = isa_spec.GetInstructions();
        // Use the instructions object...
    }
    return 0;
}
```

---

### `const std::map<std::string, OperandType>& GetOperandTypes() const noexcept`

Retrieves the operand types defined in the ISA.

#### Return Value

Returns a `const` reference to a map where the key is the name of the operand type, and the value is the corresponding `OperandType` object.

#### Example

```c++
#include "amdisa/isa_explorer.h"

int main() {
    Spec isa_spec;
    std::string err_message;
    if (isa_spec.Init("path/to/isa_spec.xml", err_message)) {
        const auto& operand_types = isa_spec.GetOperandTypes();
        // Use the operand_types object...
    }
    return 0;
}
```
