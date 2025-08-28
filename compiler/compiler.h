#ifndef COMPILER_H
#define COMPILER_H
#include "../ast/bytecode.h"
#include "../ast/type_inference.h"
#include <stdio.h>
#include <stdlib.h>  
const char* get_variable_name(ByteCodeGen* gen, int index);
void compile_to_cpp_with_types(ByteCodeGen* gen, TypeInferenceContext* ctx, FILE* output_file);
void bytecode_to_cpp(ByteCodeGen* gen, ByteCode* bytecode, FILE* output_file, TypeInferenceContext* ctx);
int compile_vbtc_to_cpp(const char* vbtc_filename, const char* cpp_filename);

#endif