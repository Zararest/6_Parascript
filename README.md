# Parascript language compiler

## Description
This repository implements compiler, that produces ASM code for processor simulator(https://github.com/Zararest/2_CPU). The language supports the basic primitives that make it turing complete and and function calls.

## Implementation
Recursive descent is used to construct the AST. Then the analysis of function calls and the creation of ASM code(`./bin/asm.txt`) is carried out.
