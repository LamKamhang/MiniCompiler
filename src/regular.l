%{
#include <stdlib.h>
#include <stdio.h>
%}
number  -?[1-9][0-9]*(\.[0-9]*)?
operator [+-*/^\(\)]
space \s+
%%
%{ 
%}
{number} { 
    yylval = strdup(yytext);
    return NUM;
}
{space} { } // space are discarded
{operator} {
    yylval = strdup(yytext);
    return yylval;
}
%%
int yywrap(){
    return 1;
}
