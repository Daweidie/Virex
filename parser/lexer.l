%{
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "../ast/ast.h"
extern YYSTYPE yylval;
char* my_strndup(const char* str, size_t n) {
    size_t len = strlen(str);
    if (n < len) {
        len = n;
    }
    
    char* result = malloc(len + 1);
    if (result) {
        memcpy(result, str, len);
        result[len] = '\0';
    }
    return result;
}
%}

%option noyywrap
%option yylineno

%%

[ \t\r\n]           ; 

"print"             return PRINT;
"i32"               return TYPE_I32;
"i64"               return TYPE_I64;
"f32"               return TYPE_F32;
"f64"               return TYPE_F64;
"str"               return TYPE_STR;

"="                 return ASSIGN;
"+"                 return PLUS;
"-"                 return MINUS;
"*"                 return MULTIPLY;
"/"                 return DIVIDE;
"%"                 return MODULO;
"**"                return POWER;

"("                 return LPAREN;
")"                 return RPAREN;

";"                 return SEMICOLON;
","                 return COMMA;


[a-zA-Z_][a-zA-Z0-9_]*  {
                        yylval.str = strdup(yytext);
                        return IDENTIFIER;
                      }

[0-9]+              {
                        yylval.num_int = atoll(yytext);
                        return NUMBER_INT;
                      }

[0-9]+"."[0-9]*      {
                        yylval.num_float = atof(yytext);
                        return NUMBER_FLOAT;
                      }

\"[^\"\n]*\"        {
                        yylval.str = my_strndup(yytext + 1, yyleng - 2);
                        return STRING;
                      }

\'[^\'\n]*\'        {
                        yylval.str = my_strndup(yytext + 1, yyleng - 2);
                        return STRING;
                      }

"//".*              ; 

"/"[*]([^*]|[*]+[^*/])*[*]+"/"  ; 

.                   {
                        fprintf(stderr, "Unknown character: %c at line %d\n", yytext[0], yylineno);
                        return ERROR;
                    }

%%