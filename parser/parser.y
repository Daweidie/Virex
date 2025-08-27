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
%token PRINT VAR TYPE_I32 TYPE_I64 TYPE_F32 TYPE_F64 TYPE_STR
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE MODULO POWER
%token LPAREN RPAREN SEMICOLON
%token ERROR

%type <node> program statement_list statement 
%type <node> print_statement assignment_statement var_declaration
%type <node> expression term factor power factor_unary
%type <node> literal identifier

%start program

%%

program
    : statement_list { root = $$ = $1; }
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
    | var_declaration SEMICOLON     { $$ = $1; }
    | print_statement               { $$ = $1; }
    | assignment_statement          { $$ = $1; }
    | var_declaration               { $$ = $1; }
    ;

print_statement
    : PRINT expression              { $$ = create_print_node($2); }
    | PRINT LPAREN expression RPAREN { $$ = create_print_node($3); }
    ;

assignment_statement
    : identifier ASSIGN expression  { $$ = create_assign_node($1, $3); }
    ;

var_declaration
    : VAR identifier ASSIGN expression { $$ = create_assign_node($2, $4); }
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
    | factor_unary POWER power      { $$ = create_binop_node(OP_POW, $1, $3); }
    ;

factor_unary
    : PLUS factor_unary             { $$ = create_unaryop_node(OP_PLUS, $2); }
    | MINUS factor_unary            { $$ = create_unaryop_node(OP_MINUS, $2); }
    | literal                       { $$ = $1; }
    | identifier                    { $$ = $1; }
    | LPAREN expression RPAREN      { $$ = $2; }
    ;

literal
    : NUMBER_INT                    { $$ = create_num_int_node($1); }
    | NUMBER_FLOAT                  { $$ = create_num_float_node($1); }
    | STRING                        { $$ = create_string_node($1); free($1); }
    ;

identifier
    : IDENTIFIER                    { $$ = create_identifier_node($1); free($1); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s at line %d\n", s, yylineno);
}