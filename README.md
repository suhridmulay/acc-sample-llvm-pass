# ACC - Assignment 2
Writing a sample llvm pass

## About
The goal of this assignment is to write a sample LLVM pass. The pass simply runs through the (llvm IR of the) program scanning basic block by basic block and then instruction by instruction and dumps the type of instruction, the instruction itself, the lvalue of the instruction (if exists) and the operands of the instruction

## Prerequisites
The program assumes you have llvm installed on your system and an appropriately configured include path. On Ubuntu (based) systems this is as simple as running:

```shell
sudo apt install llvm llvm-dev opt clang
```

This will install llvm, the development libraries for llvm (header files and like) and opt the llvm optimiser tool (needed to run the passes)  and clang to compile C code to llvm IR for testing

## Testing
Assuming you have the prerequisites set up testing is as simple as generating the IRs and testing. Two sample programs are provided for testing. One is a recursive fibonacci definition and the other calculates the value of Pi using RNG. The output of the program is printed to stderr which can be redirected to file if desired. The output is prersented stderr because stdout contains the output IR (in the form of bitcode) from the pass, which is something we might want to preserve if we ever want to combine multiple passes.

When we want to run/test the analysis pass we have written. The first step is to generate the IRs to analyse.

To do that, use:

```shell
make ir-1
make ir-2
```

This will generate the llvm IR for the test programs one and two.

Then, to run the pass simply do:

```shell
make helloPass
make test
```

This will recompile the pass (if necessary) and then run it on the generated IRs, the output is printed to the console and for each instruction the output specifies the following parameters:

1. Instruction statement
2. OpCode of the instruction
3. The kind of instruction (if categorisation is implemented)
4. Whether the instruction is of assignment form
5. Followed by a list of operands starting from the lvalue


When done if we want to cleanup the generated files we simply run:
```shell
make clean
```
