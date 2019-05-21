%{
#include <stdio.h>
#include "parser.hpp"

#define DEBUG

%}

digit           [0-9]
letter          [a-zA-Z_]
newline         \n
identifier      {letter}+({letter}|{digit})*
string          \"([^\\"\n]|\\.)*\"
comment         \/\*([^*]*\*+[^/*])*([^*]*\*+)\/
%%

"auto"			{ return AUTO; }
"break"			{ return BREAK; }
"case"			{ return CASE; }
"char"			{ return CHAR; }
"const"			{ return CONST; }
"continue"		{ return CONTINUE; }
"default"		{ return DEFAULT; }
"do"			{ return DO; }
"double"		{ return DOUBLE; }
"else"			{ return ELSE; }
"enum"			{ return ENUM; }
"extern"		{ return EXTERN; }
"float"			{ return FLOAT; }
"for"			{ return FOR; }
"goto"			{ return GOTO; }
"if"			{ return IF; }
"int"			{ return INT; }
"long"			{ return LONG; }
"register"		{ return REGISTER; }
"return"		{ return RETURN; }
"short"			{ return SHORT; }
"signed"		{ return SIGNED; }
"sizeof"		{ return SIZEOF; }
"static"		{ return STATIC; }
"struct"		{ return STRUCT; }
"switch"		{ return SWITCH; }
"typedef"		{ return TYPEDEF; }
"union"			{ return UNION; }
"unsigned"		{ return UNSIGNED; }
"void"			{ return VOID; }
"volatile"		{ return VOLATILE; }
"while"			{ return WHILE; }

{identifier}    { return IDENTIFIER;}
{comment}       {return 1;}
{digit}+         {return 1;}
{string}		{return STRING_LITERAL; }
{newline}        {return 5;}
.                 {return 0;}
%%

int yywrap()
{
	return(1);
}

#ifdef DEBUG
int main(void)
{
	while (1)
	{
		int ret = yylex();
		if (ret == 5)
			continue;
		printf("yylex() return : %d\n", ret);
		printf("#value:%s#\n", yytext);
		printf("--------------\n");
	}
}
#endif
