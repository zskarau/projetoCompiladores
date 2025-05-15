/*
 * Declaracoes para uma calculadora avancada
 */

/* interface com o lexer */

#ifndef AST_H
#define AST_H

extern int yylineno;
void yyerror(char *s, ...);

/* tabela de símbolos */
struct symbol {
    char *name;            /* um nome de variável */
    double value;
    struct ast *func;      /* stmt para função */
    struct symlist *syms;  /* lista de argumentos */
};

/* tabela de símbolos de tamanho fixo */
#define NHASH 9997
extern struct symbol symtab[NHASH];

struct symbol *lookup(char*);

/* lista de símbolos, para uma lista de argumentos */
struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* tipos de nós
 * + - * /
 * 0–7 operadores de comparação, 04 igual, 02 menor que, 01 maior que
 * L expressão ou lista de comandos
 * I comando IF
 * W comando WHILE
 * N símbolo de referência
 * = atribuição
 * S lista de símbolos
 * F chamada de função predefinida
 * C chamada de função definida pelo usuário
 */

enum bifs {   /* funções predefinidas */
    B_sqrt = 1,
    B_exp,
    B_log,
    B_print
};

typedef enum{   /* Tree evals */
    Constant =1, //'K'
	Variable, //'N'
	Assignment, //'='

	Addition, //'+'	
	Subtraction, //'-'
	Multiplication, //'*'
	Division, //'/'	

	Greater, //'1'
	Less, //'2'
    Not_equal, //'3'
	Equal, //'4'
	Greater_equal, //'5'
	Less_equal, //'6'
    
	If_else, //'I'
	While, //'W'
	For, //'X'
	Statement, //'L'

	B_call, //'F'
	User_call, //'C'

}evaluation;

/* nós na AST */
struct ast {
    evaluation nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall {     /* função predefinida */
    evaluation nodetype;   /* tipo F */
    struct ast *l;
    enum bifs functype;
};

struct ufncall {    /* função do usuário */
    evaluation nodetype;   /* tipo C */
    struct ast *l;  /* lista de argumentos */
    struct symbol *s;
};

struct flow {       /* tipo I ou W */
    evaluation nodetype;
    struct ast *cond;   /* condição */
    struct ast *tl;     /* ramo "then" ou lista "do" */
    struct ast *el;     /* ramo opcional "else" */
    struct ast *init;   /* inicialização for */
};

struct numval {
    evaluation nodetype;   /* tipo K */
    double number;
};

struct symref {
    evaluation nodetype;   /* tipo N */
    struct symbol *s;
};

struct symasgn {
    evaluation nodetype;   /* tipo = */
    struct symbol *s;
    struct ast *v;  /* valor a ser atribuído */
};

/* construção de uma AST */
struct ast *newast(evaluation nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(evaluation cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(evaluation nodetype, struct ast *cond, struct ast *tl, struct ast *el, struct ast *init);

/* definição de uma função */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* avaliação de uma AST */
double eval(struct ast *);

/* deletar e liberar uma AST */
void treefree(struct ast *);

//Flex & Bison shinanigans
int yyparse(void);
int yylex(void);

#endif