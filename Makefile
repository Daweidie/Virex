CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -D_POSIX_C_SOURCE=200809L
BISON = bison
FLEX = flex
TARGET = virex
SOURCES = ast/ast.c parser/parser.tab.c parser/lex.yy.c ast/bytecode.c compiler/compiler.c ast/type_inference.c main.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

parser/parser.tab.c parser/parser.tab.h: parser/parser.y
	cd parser && $(BISON) -d parser.y

parser/lex.yy.c: parser/lexer.l parser/parser.tab.h
	cd parser && $(FLEX) lexer.l

ast/type_inference.o: ast/type_inference.c ast/type_inference.h ast/ast.h
	$(CC) $(CFLAGS) -c ast/type_inference.c -o ast/type_inference.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

clean:
	rm -f $(TARGET) $(OBJECTS) parser/lex.yy.c parser/parser.tab.c parser/parser.tab.h

rebuild: clean all

# Run test
test: all
	./$(TARGET) test.vix

.PHONY: all clean rebuild test