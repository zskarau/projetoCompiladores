# Detect OS
ifeq ($(OS),Windows_NT)
    FLEX = win_flex
    BISON = win_bison
    EXE = main.exe
else
    FLEX = flex
    BISON = bison
    EXE = ./main
endif
CC = gcc

# Source files
LEX_SRC = lexer.l
YACC_SRC = parser.y
C_FILES = lex.yy.c parser.tab.c ast.c
HEADERS = parser.tab.h
OUTPUT = main

# Default rule
all: run

# Run flex and bison
parser.tab.c parser.tab.h: $(YACC_SRC)
	$(BISON) -d $(YACC_SRC)

lex.yy.c: $(LEX_SRC)
	$(FLEX) $(LEX_SRC)

# Compile
$(EXE): $(C_FILES)
	$(CC) -o $(OUTPUT) $(C_FILES)

# Run the program with input.txt
run: $(EXE)
	$(EXE)

# Clean
clean:
	del /f /q *.exe *.o *.tab.c *.tab.h 2>NUL || rm -f *.exe *.o *.tab.c *.tab.h

.PHONY: all run clean