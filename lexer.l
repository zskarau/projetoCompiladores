%option noyywrap nodefault yylineno

%{
    #include "ast.h"
    #include "parser.tab.h"
%}
/* expoente float */
EXP ([Ee][+-]?[0-9]+)
%%
"+"|"-"|"*"|"/"|"="|","|";"|"("|")" { return yytext[0]; }

"&&"    { yylval.fn = Greater_equal; return AND; }
"||"    { yylval.fn = Less_equal; return OR; }

">"     { yylval.fn = Greater; return CMP; }    /* operadores de comparação, todos são token CMP */
"<"     { yylval.fn = Less; return CMP; }
"<>"    { yylval.fn = Not_equal; return CMP; }
"=="    { yylval.fn = Equal; return CMP; }
">="    { yylval.fn = Greater_equal; return CMP; }
"<="    { yylval.fn = Less_equal; return CMP; }

"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"for" { return FOR; }
"do"    { return DO; }
"let"   { return LET; }

"sqrt"  { yylval.fn = B_sqrt; return FUNC; }
"exp"   { yylval.fn = B_exp; return FUNC; }
"log"   { yylval.fn = B_log; return FUNC; }
"print" { yylval.fn = B_print; return FUNC; }


[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.s = lookup(yytext);
    return NAME;
}

[0-9]+"."[0-9]*{EXP}? {
    yylval.d = atof(yytext);
    return NUMBER;
}

"."?[0-9]+{EXP}? {
    yylval.d = atof(yytext);
    return NUMBER;
}

[ \t]+ ;

\\\n             { printf("> "); }

\n               { return EOL; }


.                { yyerror("Caractere desconhecido: %c\n", *yytext); }
%%