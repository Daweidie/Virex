#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_program_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_PROGRAM;
    node->data.program.statements = NULL;
    node->data.program.statement_count = 0;
    return node;
}

void add_statement_to_program(ASTNode* program, ASTNode* statement) {
    if (program->type != AST_PROGRAM) return;
    
    program->data.program.statement_count++;
    program->data.program.statements = realloc(
        program->data.program.statements,
        sizeof(ASTNode*) * program->data.program.statement_count
    );
    program->data.program.statements[program->data.program.statement_count - 1] = statement;
}

ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_PRINT;
    node->data.print.expr = expr;
    return node;
}

ASTNode* create_expression_list_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_EXPRESSION_LIST;
    node->data.expression_list.expressions = NULL;
    node->data.expression_list.expression_count = 0;
    return node;
}

void add_expression_to_list(ASTNode* list, ASTNode* expr) {
    if (!list || list->type != AST_EXPRESSION_LIST || !expr) return;
    
    list->data.expression_list.expression_count++;
    list->data.expression_list.expressions = realloc(
        list->data.expression_list.expressions,
        sizeof(ASTNode*) * list->data.expression_list.expression_count
    );
    list->data.expression_list.expressions[list->data.expression_list.expression_count - 1] = expr;
}

ASTNode* create_assign_node(ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGN;
    node->data.assign.left = left;
    node->data.assign.right = right;
    return node;
}

ASTNode* create_binop_node(BinOpType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINOP;
    node->data.binop.op = op;
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

ASTNode* create_unaryop_node(UnaryOpType op, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_UNARYOP;
    node->data.unaryop.op = op;
    node->data.unaryop.expr = expr;
    return node;
}

ASTNode* create_num_int_node(long long value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUM_INT;
    node->data.num_int.value = value;
    return node;
}

ASTNode* create_num_float_node(double value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUM_FLOAT;
    node->data.num_float.value = value;
    return node;
}

ASTNode* create_string_node(const char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->data.string.value = malloc(strlen(value) + 1);
    strcpy(node->data.string.value, value);
    return node;
}

ASTNode* create_identifier_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_IDENTIFIER;
    node->data.identifier.name = malloc(strlen(name) + 1);
    strcpy(node->data.identifier.name, name);
    return node;
}

ASTNode* create_type_node(NodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

void free_ast(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            for (int i = 0; i < node->data.program.statement_count; i++) {
                free_ast(node->data.program.statements[i]);
            }
            free(node->data.program.statements);
            break;
            
        case AST_PRINT:
            free_ast(node->data.print.expr);
            break;
            
        case AST_EXPRESSION_LIST:
            for (int i = 0; i < node->data.expression_list.expression_count; i++) {
                free_ast(node->data.expression_list.expressions[i]);
            }
            free(node->data.expression_list.expressions);
            break;
            
        case AST_ASSIGN:
            free_ast(node->data.assign.left);
            free_ast(node->data.assign.right);
            break;
            
        case AST_BINOP:
            free_ast(node->data.binop.left);
            free_ast(node->data.binop.right);
            break;
            
        case AST_UNARYOP:
            free_ast(node->data.unaryop.expr);
            break;
            
        case AST_STRING:
            free(node->data.string.value);
            break;
            
        case AST_IDENTIFIER:
            free(node->data.identifier.name);
            break;
            
        case AST_NUM_INT:
        case AST_NUM_FLOAT:
        case AST_TYPE_INT32:
        case AST_TYPE_INT64:
        case AST_TYPE_FLOAT32:
        case AST_TYPE_FLOAT64:
        case AST_TYPE_STRING:
            break;//排除没有动态分配内存的节点
    }
    
    free(node);
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    switch (node->type) {
        case AST_PROGRAM:
            printf("Program (%d statements)\n", node->data.program.statement_count);
            for (int i = 0; i < node->data.program.statement_count; i++) {
                print_ast(node->data.program.statements[i], indent + 1);
            }
            break;
            
        case AST_PRINT:
            printf("Print\n");
            print_ast(node->data.print.expr, indent + 1);
            break;
            
        case AST_EXPRESSION_LIST:
            printf("Expression List (%d expressions)\n", node->data.expression_list.expression_count);
            for (int i = 0; i < node->data.expression_list.expression_count; i++) {
                print_ast(node->data.expression_list.expressions[i], indent + 1);
            }
            break;
            
        case AST_ASSIGN:
            printf("Assign\n");
            print_ast(node->data.assign.left, indent + 1);
            print_ast(node->data.assign.right, indent + 1);
            break;
            
        case AST_BINOP:
            printf("BinaryOp: ");
            switch (node->data.binop.op) {
                case OP_ADD: printf("Add (+)\n"); break;
                case OP_SUB: printf("Sub (-)\n"); break;
                case OP_MUL: printf("Mul (*)\n"); break;
                case OP_DIV: printf("Div (/)\n"); break;
                case OP_MOD: printf("Mod (%%)\n"); break;
                case OP_POW: printf("Pow (**)\n"); break;
                case OP_CONCAT: printf("Concat (+)\n"); break;
                case OP_REPEAT: printf("Repeat (*)\n"); break;
            }
            print_ast(node->data.binop.left, indent + 1);
            print_ast(node->data.binop.right, indent + 1);
            break;
            
        case AST_UNARYOP:
            printf("UnaryOp: ");
            switch (node->data.unaryop.op) {
                case OP_MINUS: printf("Minus (-)\n"); break;
                case OP_PLUS: printf("Plus (+)\n"); break;
            }
            print_ast(node->data.unaryop.expr, indent + 1);
            break;
            
        case AST_NUM_INT:
            printf("Integer: %lld\n", node->data.num_int.value);
            break;
            
        case AST_NUM_FLOAT:
            printf("Float: %f\n", node->data.num_float.value);
            break;
            
        case AST_STRING:
            printf("String: \"%s\"\n", node->data.string.value);
            break;
            
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->data.identifier.name);
            break;
            
        case AST_TYPE_INT32:
            printf("Type: i32\n");
            break;
            
        case AST_TYPE_INT64:
            printf("Type: i64\n");
            break;
            
        case AST_TYPE_FLOAT32:
            printf("Type: f32\n");
            break;
            
        case AST_TYPE_FLOAT64:
            printf("Type: f64\n");
            break;
            
        case AST_TYPE_STRING:
            printf("Type: str\n");
            break;
    }
}