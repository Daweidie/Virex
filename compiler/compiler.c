#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_STACK 1024
static int tmp_counter = 0;
static int stack_tmp_map[MAX_STACK];
int compile_vbtc_to_cpp(const char* vbtc_filename, const char* cpp_filename) {
    FILE* vbtc_file = fopen(vbtc_filename, "r");
    if (!vbtc_file) {
        perror("Failed to open vbtc file");
        return 1;
    }

    FILE* cpp_file = fopen(cpp_filename, "w");
    if (!cpp_file) {
        perror("Failed to create cpp file");
        fclose(vbtc_file);
        return 1;
    }
    fprintf(cpp_file, "// Auto-generated C++ code from Virex bytecode\n");
    fprintf(cpp_file, "#include <iostream>\n");
    fprintf(cpp_file, "#include <string>\n");
    fprintf(cpp_file, "#include <cmath>\n");
    fprintf(cpp_file, "#include \"lib/vcore.hpp\"\n");
    fprintf(cpp_file, "#include \"lib/vtypes.hpp\"\n");
    fprintf(cpp_file, "#include \"lib/vconvert.hpp\"\n\n");
    fprintf(cpp_file, "using namespace vcore;\n");
    fprintf(cpp_file, "using namespace vtypes;\n");
    fprintf(cpp_file, "using namespace vconvert;\n\n");
    fprintf(cpp_file, "int main() {\n");
    fprintf(cpp_file, "    // Variable declarations\n");
    tmp_counter = 0;
    memset(stack_tmp_map, -1, sizeof(stack_tmp_map));
    char line[256];
    while (fgets(line, sizeof(line), vbtc_file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        if (strstr(line, "LOAD_CONST_INT") == line) {
            long long value;
            sscanf(line, "LOAD_CONST_INT %lld", &value);
            fprintf(cpp_file, "    auto tmp_%d = %lldLL;\n", tmp_counter, value);
            tmp_counter++;
        } else if (strstr(line, "LOAD_CONST_FLOAT") == line) {
            double value;
            sscanf(line, "LOAD_CONST_FLOAT %lf", &value);
            fprintf(cpp_file, "    auto tmp_%d = %f;\n", tmp_counter, value);
            tmp_counter++;
        } else if (strstr(line, "LOAD_CONST_STRING") == line) {
            char* quote_start = strchr(line, '"');
            if (quote_start) {
                char* quote_end = strrchr(line, '"');
                if (quote_end && quote_end > quote_start) {
                    *quote_end = '\0';
                    fprintf(cpp_file, "    auto tmp_%d = vtypes::VString(\"%s\");\n", tmp_counter, quote_start + 1);
                    tmp_counter++;
                }
            }
        } else if (strstr(line, "STORE_NAME") == line) {
            int var_index;
            sscanf(line, "STORE_NAME %d", &var_index);
            fprintf(cpp_file, "    auto var_%d = tmp_%d;\n", var_index, tmp_counter-1);
        } else if (strstr(line, "PRINT") == line) {
            fprintf(cpp_file, "    print(tmp_%d);\n", tmp_counter-1);
        } else if (strstr(line, "INPUT") == line) {
            fprintf(cpp_file, "    auto tmp_%d = vtypes::VString();\n", tmp_counter);
            fprintf(cpp_file, "    std::cout << tmp_%d;\n", tmp_counter);
            fprintf(cpp_file, "    std::getline(std::cin, tmp_%d);\n", tmp_counter);
            tmp_counter++;
        } else if (strstr(line, "TOINT") == line) {
            fprintf(cpp_file, "    auto tmp_%d = to_int(tmp_%d);\n", tmp_counter, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_ADD") == line) {
            fprintf(cpp_file, "    auto tmp_%d = to_vstring(tmp_%d) + to_vstring(tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_CONCAT") == line) {
            fprintf(cpp_file, "    auto tmp_%d = to_vstring(tmp_%d) + to_vstring(tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_SUB") == line) {
            fprintf(cpp_file, "    auto tmp_%d = tmp_%d - tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_MUL") == line) {
            fprintf(cpp_file, "    auto tmp_%d = tmp_%d * tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_DIV") == line) {
            fprintf(cpp_file, "    auto tmp_%d = tmp_%d / tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_MOD") == line) {
            fprintf(cpp_file, "    auto tmp_%d = fmod(tmp_%d, tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_POW") == line) {
            fprintf(cpp_file, "    auto tmp_%d = pow(tmp_%d, tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "UNARY_MINUS") == line) {
            fprintf(cpp_file, "    auto tmp_%d = -tmp_%d;\n", tmp_counter, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "UNARY_PLUS") == line) {
            fprintf(cpp_file, "    auto tmp_%d = +tmp_%d;\n", tmp_counter, tmp_counter-1);
            tmp_counter++;
        } else if (strstr(line, "BINARY_REPEAT") == line) {
            fprintf(cpp_file, "    auto tmp_%d = to_vstring(tmp_%d) * tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
        }
    }
    
    fprintf(cpp_file, "    return 0;\n");
    fprintf(cpp_file, "}\n");
    
    fclose(vbtc_file);
    fclose(cpp_file);
    
    return 0;
}

const char* get_variable_name(ByteCodeGen* gen, int index) {
    if (index >= 0 && index < gen->var_count) {
        return gen->variables[index];
    }
    return NULL;
}

void compile_to_cpp_with_types(ByteCodeGen* gen, TypeInferenceContext* ctx, FILE* output_file) {
    // 写入头文件
    fprintf(output_file, "// Auto-generated C++ code from Virex bytecode\n");
    fprintf(output_file, "#include <iostream>\n");
    fprintf(output_file, "#include <string>\n");
    fprintf(output_file, "#include <cmath>\n");
    fprintf(output_file, "#include \"lib/vcore.hpp\"\n");
    fprintf(output_file, "#include \"lib/vtypes.hpp\"\n");
    fprintf(output_file, "#include \"lib/vconvert.hpp\"\n\n");
    fprintf(output_file, "using namespace vcore;\n");
    fprintf(output_file, "using namespace vtypes;\n");
    fprintf(output_file, "using namespace vconvert;\n\n");    
    fprintf(output_file, "int main() {\n");
    if (ctx) {
        for (int i = 0; i < ctx->var_count; i++) {
            fprintf(output_file, "    %s %s;\n", type_to_cpp_string(ctx->variables[i].type), ctx->variables[i].name);
        }
    }
    tmp_counter = 0;
    memset(stack_tmp_map, -1, sizeof(stack_tmp_map));
    for (int i = 0; i < gen->bytecode->count; i++) {
        bytecode_to_cpp(gen, &gen->bytecode->codes[i], output_file, ctx);
    }
    fprintf(output_file, "    return 0;\n");
    fprintf(output_file, "}\n");
}

void bytecode_to_cpp(ByteCodeGen* gen, ByteCode* bytecode, FILE* output_file, TypeInferenceContext* ctx) {
    switch (bytecode->op) {
        case BC_LOAD_CONST_INT:
            fprintf(output_file, "    auto tmp_%d = %lldLL;\n", tmp_counter, bytecode->operand.int_value);
            tmp_counter++;
            break;
            
        case BC_LOAD_CONST_FLOAT:
            fprintf(output_file, "    auto tmp_%d = %f;\n", tmp_counter, bytecode->operand.float_value);
            tmp_counter++;
            break;
            
        case BC_LOAD_CONST_STRING:
            fprintf(output_file, "    auto tmp_%d = vtypes::VString(\"%s\");\n", tmp_counter, bytecode->operand.string_value);
            tmp_counter++;
            break;
            
        case BC_LOAD_NAME: {
            const char* varname = get_variable_name(gen, bytecode->operand.var_index);
            fprintf(output_file, "    auto tmp_%d = %s;\n", tmp_counter, varname ? varname : "/*unknown*/");
            tmp_counter++;
            break;
        }
            
        case BC_STORE_NAME: {
            const char* vname = get_variable_name(gen, bytecode->operand.var_index);
            int right_tmp_id = tmp_counter - 1;
            if (vname) {
                if (ctx) {
                    InferredType targ = get_variable_type(ctx, vname);
                    if (targ == TYPE_INTEGER) {
                        fprintf(output_file, "    %s = (long long)to_int(tmp_%d);\n", vname, right_tmp_id);
                    } else if (targ == TYPE_FLOAT) {
                        fprintf(output_file, "    %s = static_cast<float>(to_double(tmp_%d));\n", vname, right_tmp_id);
                    } else if (targ == TYPE_STRING) {
                        fprintf(output_file, "    %s = to_vstring(tmp_%d);\n", vname, right_tmp_id);
                    } else {
                        fprintf(output_file, "    %s = tmp_%d;\n", vname, right_tmp_id);
                    }
                } else {
                    fprintf(output_file, "    %s = tmp_%d;\n", vname, right_tmp_id);
                }
            } else {
                fprintf(output_file, "    /* STORE_NAME %d (no name) -> tmp_%d */\n", bytecode->operand.var_index, right_tmp_id);
            }
            break;
        }
            
        case BC_INPUT: {
            fprintf(output_file, "    auto tmp_%d = vtypes::VString();\n", tmp_counter);
            fprintf(output_file, "    std::cout << tmp_%d;\n", tmp_counter-1);
            fprintf(output_file, "    std::getline(std::cin, tmp_%d);\n", tmp_counter);
            tmp_counter++;
            break;
        }
            
        case BC_TOINT: {
            fprintf(output_file, "    auto tmp_%d = to_int(tmp_%d);\n", tmp_counter, tmp_counter-1);
            tmp_counter++;
            break;
        }
            
        case BC_PRINT:
            fprintf(output_file, "    print(tmp_%d);\n", tmp_counter-1);
            break;
            
        case BC_BINARY_ADD:
            fprintf(output_file, "    auto tmp_%d = to_vstring(tmp_%d) + to_vstring(tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_CONCAT:
            fprintf(output_file, "    auto tmp_%d = to_vstring(tmp_%d) + to_vstring(tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_SUB:
            fprintf(output_file, "    auto tmp_%d = tmp_%d - tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_MUL:
            fprintf(output_file, "    auto tmp_%d = tmp_%d * tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_DIV:
            fprintf(output_file, "    auto tmp_%d = static_cast<float>(tmp_%d) / static_cast<float>(tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_MOD:
            fprintf(output_file, "    auto tmp_%d = tmp_%d %% tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_POW:
            fprintf(output_file, "    auto tmp_%d = pow(tmp_%d, tmp_%d);\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;
            
        case BC_BINARY_REPEAT:
            fprintf(output_file, "    auto tmp_%d = to_vstring(tmp_%d) * tmp_%d;\n", tmp_counter, tmp_counter-2, tmp_counter-1);
            tmp_counter++;
            break;

        case BC_UNARY_MINUS: {
            fprintf(output_file, "    auto tmp_%d = -tmp_%d;\n", tmp_counter, tmp_counter-1);
            tmp_counter++;
            break;
        }
            
        case BC_UNARY_PLUS:
            break;
            
        default:
            fprintf(output_file, "    // Unknown bytecode instruction\n");
            break;
    }
}