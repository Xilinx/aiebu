# ISA Specification

The control sequence ISA is specified in `isa-spec.yaml`.

This folder contains a CMake project which uses the yaml file to generate an ISA documentation (in markdown format) and a header file which defined op codes, op sizes and operation parser stubs.

The generated header file is used in the DPU interpreter located in `dpu/`.

## Updating the ISA Specification
Ensure Python modules `PyYaml` and `jinja2` are installed (use `pip3 install --user <packet>` to install).

 1. Update the `isa-spec.yaml` file
 2. Build the project and run command `cmake --build . --config Release --target isa-spec-gen` from `build/Release` folder
