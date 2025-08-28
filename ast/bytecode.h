#ifndef BYTECODE_H
#define BYTECODE_H
#include "ast.h"
typedef enum {
    BC_LOAD_CONST_INT,     
    BC_LOAD_CONST_FLOAT,   
    BC_LOAD_CONST_STRING, 
    BC_LOAD_NAME,          
    BC_STORE_NAME,         
    BC_PRINT,
    BC_BINARY_ADD,         
    BC_BINARY_SUB,         
    BC_BINARY_MUL,         
    BC_BINARY_DIV,         
    BC_BINARY_MOD,         
    BC_BINARY_POW,         
    BC_BINARY_CONCAT,      
    BC_BINARY_REPEAT,      
    BC_UNARY_MINUS,        
    BC_UNARY_PLUS          
} ByteCodeInstruction;
typedef struct {
    ByteCodeInstruction op;
    union {
        long long int_value;
        double float_value;
        char* string_value;
        int var_index;
    } operand;
} ByteCode;
typedef struct {
    ByteCode* codes;
    int count;
    int capacity;
} ByteCodeList;
typedef struct {
    ByteCodeList* bytecode;
    char** variables;
    int var_count;
    int var_capacity;
} ByteCodeGen;
ByteCodeList* create_bytecode_list();
void free_bytecode_list(ByteCodeList* list);
ByteCodeGen* create_bytecode_gen();
void free_bytecode_gen(ByteCodeGen* gen);
void generate_bytecode(ByteCodeGen* gen, ASTNode* node);
int get_variable_index(ByteCodeGen* gen, const char* name);
void print_bytecode(ByteCodeList* list);

#endif