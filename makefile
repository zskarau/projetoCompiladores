# Detect OS
ifeq ($(OS),Windows_NT)
    FLEX = win_flex
    BISON = win_bison
    EXE = calculator.exe
else
    FLEX = flex
    BISON = bison
    EXE = ./calculator
endif
CC = gcc

# Source files
LEX_SRC = lexer.l
YACC_SRC = parser.y
C_FILES = lex.yy.c parser.tab.c ast.c
HEADERS = parser.tab.h

OUTPUT = calculator
INPUT = ./tests/input.calc

all: $(EXE)

parser.tab.c parser.tab.h: $(YACC_SRC)
	$(BISON) -d $(YACC_SRC)

lex.yy.c: $(LEX_SRC)
	$(FLEX) $(LEX_SRC)

# Compile
$(EXE): $(C_FILES)
	$(CC) -o $(OUTPUT) $(C_FILES)


run: $(EXE)
	$(EXE)

test: $(EXE)
	$(EXE) $(INPUT)

clean:
	del /f /q *.exe *.o *.yy.* *.tab.* 2>NUL || rm -f *.exe *.o *.yy.* *.tab.*

.PHONY: all run clean test