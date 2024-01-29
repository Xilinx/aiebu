# ISA Specification

The control sequence ISA is specified in `isa-spec.yaml`.

This folder contains a CMake project which uses the yaml file to generate an ISA documentation (in markdown format) and a header file which defined op codes, op sizes and operation parser stubs.

The generated header file is used in the DPU interpreter located in `dpu/` in the root of this repository.
The generated python file is used in the assembler/disassembler located in `tools/` in the root of this repository.

## Updating the ISA Specification
Ensure Python modules `PyYaml` and `jinja2` are installed (use `pip3 install --user <packet>` to install).

 1. Update the `isa-spec.yaml` file
 2. Build the CMake project: `mkdir build && cd build && cmake .. && make`
 3. Copy the generated header file `build/isa_stubs.h` to the `dpu/` folder and `build/isa.py` to the `tools/package/ops/` folder  in the root of this repository
 4. Update DPU interpreter code, if needed, and rebuild the firmware
