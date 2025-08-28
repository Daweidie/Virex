#include "bytecode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ByteCodeList* create_bytecode_list() {
    ByteCodeList* list = malloc(sizeof(ByteCodeList));
    list->codes = NULL;
    list->count = 0;
    list->capacity = 0;
    return list;
}

void free_bytecode_list(ByteCodeList* list) {
    if (!list) 
        return;
    for (int i = 0; i < list->count; i++) {
        if (list->codes[i].op == BC_LOAD_CONST_STRING) {
            free(list->codes[i].operand.string_value);
        }
    }
    
    free(list->codes);
    free(list);
}

ByteCodeGen* create_bytecode_gen() {
    ByteCodeGen* gen = malloc(sizeof(ByteCodeGen));
    gen->bytecode = create_bytecode_list();
    gen->variables = NULL;
    gen->var_count = 0;
    gen->var_capacity = 0;
    return gen;
}

void free_bytecode_gen(ByteCodeGen* gen) {
    if (!gen) return;
    free_bytecode_list(gen->bytecode);
    for (int i = 0; i < gen->var_count; i++) {
        free(gen->variables[i]);
    }
    free(gen->variables);
    
    free(gen);
}

int get_variable_index(ByteCodeGen* gen, const char* name) {
    for (int i = 0; i < gen->var_count; i++) {
        if (strcmp(gen->variables[i], name) == 0) {
            return i;
        }
    }
    if (gen->var_count >= gen->var_capacity) {
        gen->var_capacity = gen->var_capacity == 0 ? 10 : gen->var_capacity * 2;
        gen->variables = realloc(gen->variables, sizeof(char*) * gen->var_capacity);
    }
    
    gen->variables[gen->var_count] = malloc(strlen(name) + 1);
    strcpy(gen->variables[gen->var_count], name);
    return gen->var_count++;
}

void add_bytecode(ByteCodeGen* gen, ByteCodeInstruction op) {
    if (gen->bytecode->count >= gen->bytecode->capacity) {
        gen->bytecode->capacity = gen->bytecode->capacity == 0 ? 10 : gen->bytecode->capacity * 2;
        gen->bytecode->codes = realloc(gen->bytecode->codes, 
                                      sizeof(ByteCode) * gen->bytecode->capacity);
    }
    
    gen->bytecode->codes[gen->bytecode->count].op = op;
    gen->bytecode->count++;
}

void generate_bytecode_program(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_PROGRAM) return;
    
    for (int i = 0; i < node->data.program.statement_count; i++) {
        generate_bytecode(gen, node->data.program.statements[i]);
    }
}

void generate_bytecode_print(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_PRINT) return;
    if (node->data.print.expr->type == AST_EXPRESSION_LIST) {
        ASTNode* expr_list = node->data.print.expr;
        int cnt = expr_list->data.expression_list.expression_count;
        if (cnt <= 0) {
            return;
        }
        
        if (cnt == 1) {
            generate_bytecode(gen, expr_list->data.expression_list.expressions[0]);
            add_bytecode(gen, BC_PRINT);
            return;
        }
        generate_bytecode(gen, expr_list->data.expression_list.expressions[0]);
        for (int i = 1; i < cnt; i++) {
            add_bytecode(gen, BC_LOAD_CONST_STRING);
            gen->bytecode->codes[gen->bytecode->count - 1].operand.string_value = malloc(strlen(" ") + 1);
            strcpy(gen->bytecode->codes[gen->bytecode->count - 1].operand.string_value, " ");
            add_bytecode(gen, BC_BINARY_CONCAT);
            generate_bytecode(gen, expr_list->data.expression_list.expressions[i]);
            add_bytecode(gen, BC_BINARY_CONCAT);
        }
        add_bytecode(gen, BC_PRINT);
    } else {
        generate_bytecode(gen, node->data.print.expr);
        add_bytecode(gen, BC_PRINT);
    }
}

void generate_bytecode_input(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_INPUT) return;
    generate_bytecode(gen, node->data.input.prompt);
    add_bytecode(gen, BC_INPUT);
}

void generate_bytecode_toint(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_TOINT) return;
    generate_bytecode(gen, node->data.toint.expr);
    add_bytecode(gen, BC_TOINT);
}

void generate_bytecode_expression_list(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_EXPRESSION_LIST) return;
    for (int i = 0; i < node->data.expression_list.expression_count; i++) {
        generate_bytecode(gen, node->data.expression_list.expressions[i]);
    }
}

void generate_bytecode_assign(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_ASSIGN) return;
    generate_bytecode(gen, node->data.assign.right);
    if (node->data.assign.left->type == AST_IDENTIFIER) {
        int var_index = get_variable_index(gen, node->data.assign.left->data.identifier.name);
        add_bytecode(gen, BC_STORE_NAME);
        gen->bytecode->codes[gen->bytecode->count - 1].operand.var_index = var_index;
    }
}

void generate_bytecode_binop(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_BINOP) return;
    generate_bytecode(gen, node->data.binop.left);
    generate_bytecode(gen, node->data.binop.right);
    switch (node->data.binop.op) {
        case OP_ADD:
            add_bytecode(gen, BC_BINARY_ADD);
            break;
        case OP_SUB:
            add_bytecode(gen, BC_BINARY_SUB);
            break;
        case OP_MUL:
            add_bytecode(gen, BC_BINARY_MUL);
            break;
        case OP_DIV:
            add_bytecode(gen, BC_BINARY_DIV);
            break;
        case OP_MOD:
            add_bytecode(gen, BC_BINARY_MOD);
            break;
        case OP_POW:
            add_bytecode(gen, BC_BINARY_POW);
            break;
        case OP_CONCAT:
            add_bytecode(gen, BC_BINARY_CONCAT);
            break;
        case OP_REPEAT:
            add_bytecode(gen, BC_BINARY_REPEAT);
            break;
    }
}

void generate_bytecode_unaryop(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_UNARYOP) return;
    generate_bytecode(gen, node->data.unaryop.expr);
    switch (node->data.unaryop.op) {
        case OP_MINUS:
            add_bytecode(gen, BC_UNARY_MINUS);
            break;
        case OP_PLUS:
            add_bytecode(gen, BC_UNARY_PLUS);
            break;
    }
}

void generate_bytecode_num_int(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_NUM_INT) return;
    
    add_bytecode(gen, BC_LOAD_CONST_INT);
    gen->bytecode->codes[gen->bytecode->count - 1].operand.int_value = node->data.num_int.value;
}

void generate_bytecode_num_float(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_NUM_FLOAT) return;
    
    add_bytecode(gen, BC_LOAD_CONST_FLOAT);
    gen->bytecode->codes[gen->bytecode->count - 1].operand.float_value = node->data.num_float.value;
}

void generate_bytecode_string(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_STRING) return;
    
    add_bytecode(gen, BC_LOAD_CONST_STRING);
    gen->bytecode->codes[gen->bytecode->count - 1].operand.string_value = malloc(strlen(node->data.string.value) + 1);
    strcpy(gen->bytecode->codes[gen->bytecode->count - 1].operand.string_value, node->data.string.value);
}

void generate_bytecode_identifier(ByteCodeGen* gen, ASTNode* node) {
    if (node->type != AST_IDENTIFIER) return;
    
    int var_index = get_variable_index(gen, node->data.identifier.name);
    add_bytecode(gen, BC_LOAD_NAME);
    gen->bytecode->codes[gen->bytecode->count - 1].operand.var_index = var_index;
}

void generate_bytecode(ByteCodeGen* gen, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            generate_bytecode_program(gen, node);
            break;
        case AST_PRINT:
            generate_bytecode_print(gen, node);
            break;
        case AST_INPUT:
            generate_bytecode_input(gen, node);
            break;
        case AST_TOINT:
            generate_bytecode_toint(gen, node);
            break;
        case AST_EXPRESSION_LIST:
            generate_bytecode_expression_list(gen, node);
            break;
        case AST_ASSIGN:
            generate_bytecode_assign(gen, node);
            break;
        case AST_BINOP:
            generate_bytecode_binop(gen, node);
            break;
        case AST_UNARYOP:
            generate_bytecode_unaryop(gen, node);
            break;
        case AST_NUM_INT:
            generate_bytecode_num_int(gen, node);
            break;
        case AST_NUM_FLOAT:
            generate_bytecode_num_float(gen, node);
            break;
        case AST_STRING:
            generate_bytecode_string(gen, node);
            break;
        case AST_IDENTIFIER:
            generate_bytecode_identifier(gen, node);
            break;
        default:
            break;
    }
}

void print_bytecode(ByteCodeList* list) {
    printf("Bytecode:\n");
    for (int i = 0; i < list->count; i++) {
        printf("%4d: ", i);
        switch (list->codes[i].op) {
            case BC_LOAD_CONST_INT:
                printf("LOAD_CONST_INT %lld\n", list->codes[i].operand.int_value);
                break;
            case BC_LOAD_CONST_FLOAT:
                printf("LOAD_CONST_FLOAT %f\n", list->codes[i].operand.float_value);
                break;
            case BC_LOAD_CONST_STRING:
                printf("LOAD_CONST_STRING \"%s\"\n", list->codes[i].operand.string_value);
                break;
            case BC_LOAD_NAME:
                printf("LOAD_NAME %d\n", list->codes[i].operand.var_index);
                break;
            case BC_STORE_NAME:
                printf("STORE_NAME %d\n", list->codes[i].operand.var_index);
                break;
            case BC_INPUT:
                printf("INPUT\n");
                break;
            case BC_TOINT:
                printf("TOINT\n");
                break;
            case BC_PRINT:
                printf("PRINT\n");
                break;
            case BC_BINARY_ADD:
                printf("BINARY_ADD\n");
                break;
            case BC_BINARY_SUB:
                printf("BINARY_SUB\n");
                break;
            case BC_BINARY_MUL:
                printf("BINARY_MUL\n");
                break;
            case BC_BINARY_DIV:
                printf("BINARY_DIV\n");
                break;
            case BC_BINARY_MOD:
                printf("BINARY_MOD\n");
                break;
            case BC_BINARY_POW:
                printf("BINARY_POW\n");
                break;
            case BC_UNARY_MINUS:
                printf("UNARY_MINUS\n");
                break;
            case BC_UNARY_PLUS:
                printf("UNARY_PLUS\n");
                break;
            case BC_BINARY_CONCAT:
                printf("BINARY_CONCAT\n");
                break;
            case BC_BINARY_REPEAT:
                printf("BINARY_REPEAT\n");
                break;
        }
    }
}