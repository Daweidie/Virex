CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -D_POSIX_C_SOURCE=200809L
BISON = bison
FLEX = flex
TARGET = virex
SOURCES = ast/ast.c parser/parser.tab.c parser/lex.yy.c ast/bytecode.c main.c
OBJECTS = $(SOURCES:.c=.o)
all: $(TARGET)
parser/parser.tab.c parser/parser.tab.h: parser/parser.y
	cd parser && $(BISON) -d parser.y
parser/lex.yy.c: parser/lexer.l parser/parser.tab.h
	cd parser && $(FLEX) lexer.l
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET): parser/parser.tab.c parser/lex.yy.c $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lm
clean:
	rm -f parser/parser.tab.c parser/parser.tab.h parser/lex.yy.c
	rm -f $(OBJECTS) $(TARGET)
rebuild: clean all
.PHONY: all clean rebuild