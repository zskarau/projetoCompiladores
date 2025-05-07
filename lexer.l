%{
#include "parser.tab.h"
#include <stdlib.h>
%}
%%
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
[0-9]+  { yylval.num = atof(yytext); return NUMBER; }
\n      { return EOL; }
[ \t]   { /* ignora espaço em branco */ }
.       { printf("Caracter não reconhecido: %c\n", *yytext); }
%%
int yywrap(void) {
    return 1;
}