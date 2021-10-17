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
