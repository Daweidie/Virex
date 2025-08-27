#ifndef PARSER_H
#define PARSER_H
#include "../ast/ast.h"
int yylex(void);
int yyparse(void);
void yyerror(const char *s);
extern ASTNode* root;
#endif
