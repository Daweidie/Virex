#ifndef AST_H
#define AST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct ASTNode ASTNode;

typedef enum {
    AST_PROGRAM,
    AST_PRINT,
    AST_ASSIGN,
    AST_BINOP,
    AST_UNARYOP,
    AST_NUM_INT,
    AST_NUM_FLOAT,
    AST_STRING,
    AST_IDENTIFIER,
    AST_TYPE_INT32,
    AST_TYPE_INT64,
    AST_TYPE_FLOAT32,
    AST_TYPE_FLOAT64,
    AST_TYPE_STRING
} NodeType;

typedef enum {
    OP_ADD,      // +
    OP_SUB,      // -
    OP_MUL,      // *
    OP_DIV,      // /
    OP_MOD,      // %
    OP_POW       // **
} BinOpType;

typedef enum {
    OP_MINUS,    // -
    OP_PLUS      // +
} UnaryOpType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode** statements;
            int statement_count;
        } program;
        struct {
            struct ASTNode* expr;
        } print;
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
        } assign;
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            BinOpType op;
        } binop;
        struct {
            struct ASTNode* expr;
            UnaryOpType op;
        } unaryop;
        struct {
            long long value;
        } num_int;
        struct {
            double value;
        } num_float;
        struct {
            char* value;
        } string;
        struct {
            char* name;
        } identifier;
    } data;
} ASTNode;

ASTNode* create_program_node();
void add_statement_to_program(ASTNode* program, ASTNode* statement);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_assign_node(ASTNode* left, ASTNode* right);
ASTNode* create_binop_node(BinOpType op, ASTNode* left, ASTNode* right);
ASTNode* create_unaryop_node(UnaryOpType op, ASTNode* expr);
ASTNode* create_num_int_node(long long value);
ASTNode* create_num_float_node(double value);
ASTNode* create_string_node(const char* value);
ASTNode* create_identifier_node(const char* name);
ASTNode* create_type_node(NodeType type);
void free_ast(ASTNode* node);
void print_ast(ASTNode* node, int indent);

#endif