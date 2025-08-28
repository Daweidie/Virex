%{
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"
extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
void yyerror(const char* s);
ASTNode* root;
%}

%union {
    long long num_int;
    double num_float;
    char* str;
    struct ASTNode* node;
}

%token <str> IDENTIFIER STRING
%token <num_int> NUMBER_INT
%token <num_float> NUMBER_FLOAT
%token PRINT INPUT TOINT TYPE_I32 TYPE_I64 TYPE_F32 TYPE_F64 TYPE_STR
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE MODULO POWER
%token LPAREN RPAREN SEMICOLON COMMA
%token ERROR

%type <node> program statement_list statement 
%type <node> print_statement assignment_statement input_statement
%type <node> expression expression_list term factor power factor_unary
%type <node> literal identifier toint_expression

%start program

%%

program
    : statement_list { root = $$ = $1; }
    ;

toint_expression
    : TOINT LPAREN expression RPAREN { $$ = create_toint_node($3); }
    ;

statement_list
    : statement                  { 
                                   $$ = create_program_node();
                                   add_statement_to_program($$, $1);
                               }
    | statement_list statement   {
                                   add_statement_to_program($1, $2);
                                   $$ = $1;
                               }
    ;

statement
    : print_statement SEMICOLON     { $$ = $1; }
    | assignment_statement SEMICOLON { $$ = $1; }
    | input_statement SEMICOLON     { $$ = $1; }
    | print_statement               { $$ = $1; }
    | assignment_statement          { $$ = $1; }
    | input_statement               { $$ = $1; }
    ;

print_statement
    : PRINT expression              { $$ = create_print_node($2); }
    | PRINT LPAREN expression RPAREN { $$ = create_print_node($3); }
    | PRINT LPAREN expression_list RPAREN { $$ = create_print_node($3); }
    ;

assignment_statement
    : identifier ASSIGN expression  { $$ = create_assign_node($1, $3); }
    ;

input_statement
    : identifier ASSIGN INPUT LPAREN STRING RPAREN { 
        ASTNode* prompt = create_string_node($5);
        $$ = create_assign_node($1, create_input_node(prompt)); 
    }
    | identifier ASSIGN INPUT LPAREN RPAREN { 
        ASTNode* prompt = create_string_node("");
        $$ = create_assign_node($1, create_input_node(prompt)); 
    }
    ;

expression_list
    : expression COMMA expression   { 
                                      ASTNode* list = create_expression_list_node();
                                      add_expression_to_list(list, $1);
                                      add_expression_to_list(list, $3);
                                      $$ = list;
                                    }
    | expression_list COMMA expression { 
                                      add_expression_to_list($1, $3);
                                      $$ = $1;
                                    }
    ;

expression
    : term                          { $$ = $1; }
    | expression PLUS term          { $$ = create_binop_node(OP_ADD, $1, $3); }
    | expression MINUS term         { $$ = create_binop_node(OP_SUB, $1, $3); }
    ;

term
    : factor                        { $$ = $1; }
    | term MULTIPLY factor          { $$ = create_binop_node(OP_MUL, $1, $3); }
    | term DIVIDE factor            { $$ = create_binop_node(OP_DIV, $1, $3); }
    | term MODULO factor            { $$ = create_binop_node(OP_MOD, $1, $3); }
    ;

factor
    : power                         { $$ = $1; }
    ;

power
    : factor_unary                  { $$ = $1; }
    | factor_unary POWER factor     { $$ = create_binop_node(OP_POW, $1, $3); }
    ;

factor_unary
    : literal                       { $$ = $1; }
    | identifier                    { $$ = $1; }
    | toint_expression              { $$ = $1; }
    | PLUS factor_unary             { $$ = create_unaryop_node(OP_PLUS, $2); }
    | MINUS factor_unary            { $$ = create_unaryop_node(OP_MINUS, $2); }
    | LPAREN expression RPAREN      { $$ = $2; }
    ;

literal
    : NUMBER_INT                    { $$ = create_num_int_node($1); }
    | NUMBER_FLOAT                  { $$ = create_num_float_node($1); }
    | STRING                        { $$ = create_string_node($1); }
    ;

identifier
    : IDENTIFIER                    { $$ = create_identifier_node($1); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s at line %d\n", s, yylineno);
}