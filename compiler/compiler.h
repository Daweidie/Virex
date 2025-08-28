#ifndef COMPILER_H
#define COMPILER_H
#include "../ast/bytecode.h"
#include "../ast/type_inference.h"
#include <stdio.h>
void compile_to_cpp(ByteCodeGen* gen, FILE* output_file);
const char* get_variable_name(ByteCodeGen* gen, int index);
int bytecode_to_cpp(ByteCodeGen* gen, ByteCode* bytecode, int stack_top, FILE* output_file);
void compile_to_cpp_with_types(ByteCodeGen* gen, TypeInferenceContext* ctx, FILE* output_file);
#endif