#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "ast.h"

struct symbol symtab[NHASH];

// Funcao de hashing
static unsigned symhash(char *sym) {
    unsigned int hash = 0;
    unsigned c;
    while ((c = *sym++))
        hash = hash * 9 ^ c;
    return hash;
}

// Procura símbolo na tabela de símbolos
struct symbol *lookup(char *sym) {
    struct symbol *sp = &symtab[symhash(sym) % NHASH];
    int scount = NHASH;

    while (--scount >= 0) {
        if (sp->name && !strcasecmp(sp->name, sym))
            return sp;

        if (!sp->name) {
            sp->name = strdup(sym);
            sp->value = 0;
            sp->func = NULL;
            sp->syms = NULL;
            return sp;
        }

        if (++sp >= symtab + NHASH)
            sp = symtab;
    }

    yyerror("overflow na tabela de símbolos");
    abort();
}

// Criacao de nós AST
struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *a = malloc(sizeof(struct ast));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast *newnum(double d) {
    struct numval *a = malloc(sizeof(struct numval));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = 'K';
    a->number = d;
    return (struct ast *)a;
}

struct ast *newcmp(int cmptype, struct ast *l, struct ast *r) {
    struct ast *a = malloc(sizeof(struct ast));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = '0' + cmptype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast *newfunc(int functype, struct ast *l) {
    struct fncall *a = malloc(sizeof(struct fncall));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = 'F';
    a->l = l;
    a->functype = functype;
    return (struct ast *)a;
}

struct ast *newcall(struct symbol *s, struct ast *l) {
    struct ufncall *a = malloc(sizeof(struct ufncall));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = 'C';
    a->l = l;
    a->s = s;
    return (struct ast *)a;
}

struct ast *newref(struct symbol *s) {
    struct symref *a = malloc(sizeof(struct symref));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = 'N';
    a->s = s;
    return (struct ast *)a;
}

struct ast *newasgn(struct symbol *s, struct ast *v) {
    struct symasgn *a = malloc(sizeof(struct symasgn));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = '=';
    a->s = s;
    a->v = v;
    return (struct ast *)a;
}

struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el) {
    struct flow *a = malloc(sizeof(struct flow));
    if (!a) {
        yyerror("sem espaco");
        exit(0);
    }
    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;
    return (struct ast *)a;
}

// Libera uma AST
void treefree(struct ast *a) {
    switch(a->nodetype) {
        // duas subarvores
        case '+': case '-': case '*': case '/':
        case '1': case '2': case '3': case '4':
        case '5': case '6': case 'L':
            treefree(a->r);
            // cair para o caso de uma subarvore
        case 'F': case 'C':
            treefree(a->l);
            break;
        case 'K': case 'N':
            break;
        case '=':
            free(((struct symasgn *)a)->v);
            break;
        case 'I': case 'W': {
            struct flow *f = (struct flow *)a;
            treefree(f->cond);
            if (f->tl) treefree(f->tl);
            if (f->el) treefree(f->el);
            break;
        }
        default:
            printf("erro interno: bad node %c\n", a->nodetype);
    }

    free(a);
}

// Lista de símbolos
struct symlist *newsymlist(struct symbol *sym, struct symlist *next) {
    struct symlist *sl = malloc(sizeof(struct symlist));
    if (!sl) {
        yyerror("sem espaco");
        exit(0);
    }
    sl->sym = sym;
    sl->next = next;
    return sl;
}

void symlistfree(struct symlist *sl) {
    struct symlist *nsl;
    while (sl) {
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

// Avaliacao de AST
static double callbuiltin(struct fncall *);
static double calluser(struct ufncall *);

double eval(struct ast *a) {
    double v;

    if (!a) {
        yyerror("erro interno: null eval");
        return 0.0;
    }

    switch (a->nodetype) {
        case 'K':
            v = ((struct numval *)a)->number;
            break;
        case 'N':
            v = ((struct symref *)a)->s->value;
            break;
        case '=':
            v = ((struct symasgn *)a)->s->value = eval(((struct symasgn *)a)->v);
            break;
        case '+':
            v = eval(a->l) + eval(a->r);
            break;
        case '-':
            v = eval(a->l) - eval(a->r);
            break;
        case '*':
            v = eval(a->l) * eval(a->r);
            break;
        case '/':
            v = eval(a->l) / eval(a->r);
            break;
        case '1':
            v = (eval(a->l) > eval(a->r)) ? 1 : 0;
            break;
        case '2':
            v = (eval(a->l) < eval(a->r)) ? 1 : 0;
            break;
        case '3':
            v = (eval(a->l) != eval(a->r)) ? 1 : 0;
            break;
        case '4':
            v = (eval(a->l) == eval(a->r)) ? 1 : 0;
            break;
        case '5':
            v = (eval(a->l) >= eval(a->r)) ? 1 : 0;
            break;
        case '6':
            v = (eval(a->l) <= eval(a->r)) ? 1 : 0;
            break;
        case 'I': {
            struct flow *f = (struct flow *)a;
            if (eval(f->cond) != 0) {
                v = f->tl ? eval(f->tl) : 0.0;
            } else {
                v = f->el ? eval(f->el) : 0.0;
            }
            break;
        }
        case 'W': {
            struct flow *f = (struct flow *)a;
            v = 0.0;
            if (f->tl) {
                while (eval(f->cond) != 0)
                    v = eval(f->tl);
            }
            break;
        }
        case 'L':
            eval(a->l);
            v = eval(a->r);
            break;
        case 'F':
            v = callbuiltin((struct fncall *)a);
            break;
        case 'C':
            v = calluser((struct ufncall *)a);
            break;
        default:
            printf("erro interno: bad node %c\n", a->nodetype);
    }

    return v;
}

// Funcoes built-in
static double callbuiltin(struct fncall *f) {
    enum bifs functype = f->functype;
    double v = eval(f->l);

    switch (functype) {
        case B_sqrt:
            return sqrt(v);
        case B_exp:
            return exp(v);
        case B_log:
            return log(v);
        case B_print:
            printf("= %4.4g\n", v);
            return v;
        default:
            yyerror("Funcao pre-definida %d desconhecida\n", functype);
            return 0.0;
    }
}

// Definicao de funcao do usuario
void dodef(struct symbol *name, struct symlist *syms, struct ast *func) {
    if (name->syms)
        symlistfree(name->syms);
    if (name->func)
        treefree(name->func);

    name->syms = syms;
    name->func = func;
}

// Chamada de funcao do usuario
static double calluser(struct ufncall *f) {
    struct symbol *fn = f->s;
    struct symlist *sl;
    struct ast *args = f->l;
    double *oldval, *newval;
    double v;
    int nargs, i;

    if (!fn->func) {
        yyerror("Chamada para funcao %s indefinida", fn->name);
        return 0.0;
    }

    sl = fn->syms;
    for (nargs = 0; sl; sl = sl->next)
        nargs++;

    oldval = malloc(nargs * sizeof(double));
    newval = malloc(nargs * sizeof(double));
    if (!oldval || !newval) {
        yyerror("Sem espaco em %s", fn->name);
        return 0.0;
    }

    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        if (!args) {
            yyerror("Poucos argumentos na chamada da funcao %s", fn->name);
            free(oldval);
            free(newval);
            return 0.0;
        }

        if (args->nodetype == 'L') {
            newval[i] = eval(args->l);
            args = args->r;
        } else {
            newval[i] = eval(args);
            args = NULL;
        }
    }

    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        struct symbol *s = sl->sym;
        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }

    free(newval);

    v = eval(fn->func);

    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        struct symbol *s = sl->sym;
        s->value = oldval[i];
        sl = sl->next;
    }

    free(oldval);
    return v;
}

// Funcao de erro
void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

// Funcao principal
int main() {
    printf("> ");
    return yyparse();
}
