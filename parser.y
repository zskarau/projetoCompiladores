/* calculadora simples – parser */
%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%union{
    long double num;
}


/* declaracao de tokens */
%token <num> NUMBER
%token ADD SUB MUL DIV
%token EOL

%type <num> factor exp term
%%

calclist: /* empty */
        | calclist exp EOL { printf("= %g\n", $2); } /* EOL end of an expression */
        ;

exp: factor              /* default $$ = $1 */
        | exp ADD factor { $$ = $1 + $3; }
        | exp SUB factor { $$ = $1 - $3; }
        ;

factor: term             /* default $$ = $1 */
        | factor MUL term { $$ = $1 * $3; }
        | factor DIV term { if($3 == 0){ printf("erro divisão por zero"); } $$ = $1 / $3; }
        ;

term: NUMBER             /* default $$ = $1 */
        ;
%%

int main(int argc, char **argv)
{
    yyparse();
}

void yyerror(const char *s)
{
    fprintf(stderr, "error: %s\n", s);
}
