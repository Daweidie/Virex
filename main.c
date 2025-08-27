#include <stdio.h>
#include <stdlib.h>
#include "parser/parser.h"
#include "ast/ast.h"
#include "ast/bytecode.h"

extern FILE* yyin;
extern ASTNode* root;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件.vix>\n", argv[0]);
        return 1;
    }
    
    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("文件打开失败！");
        return 1;
    }
    
    yyin = input_file;
    
    int result = yyparse();
    
    if (result == 0) {
        printf("语法解析成功!\n");
        printf("由bison和flex生成的AST:\n");
        print_ast(root, 0);
        
        // 生成字节码
        ByteCodeGen* gen = create_bytecode_gen();
        generate_bytecode(gen, root);
        print_bytecode(gen->bytecode);
        free_bytecode_gen(gen);
    } else {
        printf("语法解析失败！\n");
    }
    if (root) {
        free_ast(root);
    }
    
    fclose(input_file);
    return result;
}