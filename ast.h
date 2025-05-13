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

/* nós na AST */
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall {     /* função predefinida */
    int nodetype;   /* tipo F */
    struct ast *l;
    enum bifs functype;
};

struct ufncall {    /* função do usuário */
    int nodetype;   /* tipo C */
    struct ast *l;  /* lista de argumentos */
    struct symbol *s;
};

struct flow {       /* tipo I ou W */
    int nodetype;
    struct ast *cond;   /* condição */
    struct ast *tl;     /* ramo "then" ou lista "do" */
    struct ast *el;     /* ramo opcional "else" */
};

struct numval {
    int nodetype;   /* tipo K */
    double number;
};

struct symref {
    int nodetype;   /* tipo N */
    struct symbol *s;
};

struct symasgn {
    int nodetype;   /* tipo = */
    struct symbol *s;
    struct ast *v;  /* valor a ser atribuído */
};

/* construção de uma AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* definição de uma função */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* avaliação de uma AST */
double eval(struct ast *);

/* deletar e liberar uma AST */
void treefree(struct ast *);

#endif