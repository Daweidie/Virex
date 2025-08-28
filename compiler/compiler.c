#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_STACK 1024
static int tmp_counter = 0;//计数器
static int stack_tmp_map[MAX_STACK];

const char* get_variable_name(ByteCodeGen* gen, int index) {
    if (index >= 0 && index < gen->var_count) {
        return gen->variables[index];
    }
    return NULL;
}

void compile_to_cpp(ByteCodeGen* gen, FILE* output_file) {
    // 写入头文件
    fprintf(output_file, "// Auto-generated C++ code from Virex bytecode\n");
    fprintf(output_file, "#include <iostream>\n");
    fprintf(output_file, "#include <string>\n");
    fprintf(output_file, "#include <cmath>\n");
    fprintf(output_file, "#include \"lib/vcore.hpp\"\n");
    fprintf(output_file, "#include \"lib/vtypes.hpp\"\n\n");
    fprintf(output_file, "using namespace vcore;\n");
    fprintf(output_file, "using namespace vtypes;\n\n");
    fprintf(output_file, "int main() {\n");
    tmp_counter = 0;
    for (int i = 0; i < MAX_STACK; i++) stack_tmp_map[i] = -1;
    for (int i = 0; i < gen->var_count; i++) {
        fprintf(output_file, "    auto %s = 0;\n", gen->variables[i]);
    }
    
    if (gen->var_count > 0) {
        fprintf(output_file, "\n");
    }
    int stack_top = 0;
    for (int i = 0; i < gen->bytecode->count; i++) {
        stack_top = bytecode_to_cpp(gen, &gen->bytecode->codes[i], stack_top, output_file);
    }
    fprintf(output_file, "    \n");
    fprintf(output_file, "    return 0;\n");
    fprintf(output_file, "}\n");
}

void compile_to_cpp_with_types(ByteCodeGen* gen, TypeInferenceContext* ctx, FILE* output_file) {
    fprintf(output_file, "// Auto-generated C++ code from Virex bytecode with type inference\n");
    fprintf(output_file, "#include <iostream>\n");
    fprintf(output_file, "#include <string>\n");
    fprintf(output_file, "#include <cmath>\n");
    fprintf(output_file, "#include \"lib/vcore.hpp\"\n");
    fprintf(output_file, "#include \"lib/vtypes.hpp\"\n\n");
    fprintf(output_file, "using namespace vcore;\n");
    fprintf(output_file, "using namespace vtypes;\n\n");
    fprintf(output_file, "int main() {\n");
    tmp_counter = 0;
    for (int i = 0; i < MAX_STACK; i++) stack_tmp_map[i] = -1;
    for (int i = 0; i < gen->var_count; i++) {
        InferredType var_type = get_variable_type(ctx, gen->variables[i]);
        const char* type_str = type_to_cpp_string(var_type);
        if (var_type == TYPE_STRING) {
            fprintf(output_file, "    %s %s = \"\";\n", type_str, gen->variables[i]);
        } else if (var_type == TYPE_FLOAT) {
            fprintf(output_file, "    %s %s = 0.0;\n", type_str, gen->variables[i]);
        } else if (var_type == TYPE_INTEGER) {
            fprintf(output_file, "    %s %s = 0;\n", type_str, gen->variables[i]);
        } else {
            fprintf(output_file, "    auto %s = 0;\n", gen->variables[i]);
        }
    }
    
    if (gen->var_count > 0) {
        fprintf(output_file, "\n");
    }
    int stack_top = 0;
    for (int i = 0; i < gen->bytecode->count; i++) {
        stack_top = bytecode_to_cpp(gen, &gen->bytecode->codes[i], stack_top, output_file);
    }
    fprintf(output_file, "    \n");
    fprintf(output_file, "    return 0;\n");
    fprintf(output_file, "}\n");
}
int bytecode_to_cpp(ByteCodeGen* gen, ByteCode* bytecode, int stack_top, FILE* output_file) {
    int left_id, right_id, new_id;
    switch (bytecode->op) {
        case BC_LOAD_CONST_INT:
            new_id = tmp_counter++;
            if (stack_top < MAX_STACK) stack_tmp_map[stack_top] = new_id;
            fprintf(output_file, "    auto tmp_%d = %lldLL;\n", new_id, bytecode->operand.int_value);
            return stack_top + 1;
            
        case BC_LOAD_CONST_FLOAT:
            new_id = tmp_counter++;
            if (stack_top < MAX_STACK) stack_tmp_map[stack_top] = new_id;
            fprintf(output_file, "    auto tmp_%d = %f;\n", new_id, bytecode->operand.float_value);
            return stack_top + 1;
            
        case BC_LOAD_CONST_STRING:
            new_id = tmp_counter++;
            if (stack_top < MAX_STACK) stack_tmp_map[stack_top] = new_id;
            fprintf(output_file, "    VString tmp_%d = \"%s\";\n", new_id, bytecode->operand.string_value);
            return stack_top + 1;
            
        case BC_LOAD_NAME: {
            new_id = tmp_counter++;
            if (stack_top < MAX_STACK) stack_tmp_map[stack_top] = new_id;
            const char* varname = get_variable_name(gen, bytecode->operand.var_index);
            fprintf(output_file, "    auto tmp_%d = %s;\n", new_id, varname ? varname : "/*unknown*/");
            return stack_top + 1;
        }
            
        case BC_STORE_NAME:
            right_id = stack_tmp_map[stack_top - 1];
            fprintf(output_file, "    %s = tmp_%d;\n", get_variable_name(gen, bytecode->operand.var_index), right_id);
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            return stack_top - 1;
            
        case BC_PRINT:
            right_id = stack_tmp_map[stack_top - 1];
            fprintf(output_file, "    print(tmp_%d);\n", right_id);
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            return stack_top - 1;
            
        case BC_BINARY_ADD:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d + tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_SUB:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d - tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_MUL:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d * tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_DIV:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d / tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_MOD:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d %% tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_POW:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = pow(tmp_%d, tmp_%d);\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_CONCAT:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d + tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;
            
        case BC_BINARY_REPEAT:
            left_id = stack_tmp_map[stack_top - 2];
            right_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 2 >= 0 && stack_top - 2 < MAX_STACK) stack_tmp_map[stack_top - 2] = new_id;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = -1;
            fprintf(output_file, "    auto tmp_%d = tmp_%d * tmp_%d;\n", new_id, left_id, right_id);
            return stack_top - 1;

        case BC_UNARY_MINUS: {
            int operand_id = stack_tmp_map[stack_top - 1];
            new_id = tmp_counter++;
            if (stack_top - 1 >= 0 && stack_top - 1 < MAX_STACK) stack_tmp_map[stack_top - 1] = new_id;
            fprintf(output_file, "    auto tmp_%d = -tmp_%d;\n", new_id, operand_id);
            return stack_top;
        }
            
        case BC_UNARY_PLUS:
            return stack_top;
            
        default:
            fprintf(output_file, "    // Unknown bytecode instruction\n");
            return stack_top;
    }
}