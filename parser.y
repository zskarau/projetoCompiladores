%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
%}


%union {
    struct ast *a;
    long double d;
    struct symbol *s;   /* qual símbolo? */
    struct symlist *sl;
    int fn;             /* qual função? */
}

/* declaração de tokens */
%token <d> NUMBER
%token <s> NAME
%token <fn> FUNC
%token EOL

%token IF THEN ELSE WHILE DO LET FOR AND OR

%nonassoc <fn> CMP
%right '='
%left AND OR
%left '+' '-'
%left '*' '/'

%type <a> exp stmt list explist init cond inc
%type <sl> symlist

%start calclist
%%

stmt:
      IF exp THEN list                { $$ = newflow(If_else, $2, $4, NULL, NULL); }
    | IF exp THEN list ELSE list      { $$ = newflow(If_else, $2, $4, $6, NULL); }
    | WHILE exp DO list               { $$ = newflow(While, $2, $4, NULL, NULL); }
    | FOR '(' init  ';'cond ';' inc ')' list { $$ = newflow(For, $5, $9, $7, $3); }
    | exp
;

init: NAME '=' exp      { $$ = newasgn($1, $3); };
cond:  exp CMP exp      { $$ = newcmp($2, $1, $3); };
inc: NAME '=' exp       { $$ = newasgn($1, $3); };

list:
      /* vazio! */                   { $$ = NULL; }
    | stmt ';' list {
          if ($3 == NULL)
              $$ = $1;
          else
              $$ = newast(Statement, $1, $3);
      }
;

exp:
      exp CMP exp        { $$ = newcmp($2, $1, $3); }
    | exp '+' exp        { $$ = newast(Addition, $1, $3); }
    | exp '-' exp        { $$ = newast(Subtraction, $1, $3); }
    | exp '*' exp        { $$ = newast(Multiplication, $1, $3); }
    | exp '/' exp        { $$ = newast(Division, $1, $3); }
    | exp AND exp       { $$ = newast(And, $1, $3); }
    | exp OR exp       { $$ = newast(Or, $1, $3); }
    | '(' exp ')'        { $$ = $2; }
    | NUMBER             { $$ = newnum($1); }
    | NAME               { $$ = newref($1); }
    | NAME '=' exp       { $$ = newasgn($1, $3); }
    | FUNC '(' explist ')' { $$ = newfunc($1, $3); }
    | NAME '(' explist ')' { $$ = newcall($1, $3); }
;

explist:
      exp                { $$ = $1; }
    | exp ',' explist    { $$ = newast(Statement, $1, $3); }
;

symlist:
      NAME               { $$ = newsymlist($1, NULL); }
    | NAME ',' symlist   { $$ = newsymlist($1, $3); }
;

calclist:
      /* vazio! */
    | calclist stmt EOL {
          printf("= %g\n", (double)eval($2));
          treefree($2);
      }
    | calclist LET NAME '(' symlist ')' '=' list EOL {
          dodef($3, $5, $8);
          printf("Defined %s\n", $3->name);
      }
    | calclist error EOL { yyerrok; printf(">\n"); }
;
