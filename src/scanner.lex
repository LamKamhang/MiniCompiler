%{
#include <stdio.h>
#include "parser.hpp"

#define DEBUG

%}

digit           [0-9]
letter          [a-zA-Z_]
newline         \n
whitespace      [\t ]+
identifier      {letter}+({letter}|{digit})*
string          \"([^\\"\n]|\\.)*\"
comment         \/\*([^*]*\*+[^/*])*([^*]*\*+)\/

num             {digit}+
hex_num         0[xX][a-fA-F0-9]+
float_num       ({digit}*\.{digit}+)|({digit}+\.{digit}*)
e_num           {num}e[+]?{num}
e_float         ({num}e-{num})|({float_num}e[+-]?{num})

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
{comment}       {/*this is a comment.*/}
{whitespace}    {/*this is a whitespace*/}
{newline}       {/*this is a newline*/return 5;}
{string}		{return STRING_LITERAL; }

{num}           { return CONSTANT_INT;}
{hex_num}           { return CONSTANT_INT;}
{float_num}           { return CONSTANT_FLOAT;}
{e_num}           { return CONSTANT_INT;}
{e_float}         { return CONSTANT_FLOAT;}


"..."			{ return ELLIPSIS; }
">>="			{ return RIGHT_ASSIGN; }
"<<="			{ return LEFT_ASSIGN; }
"+="			{ return ADD_ASSIGN; }
"-="			{ return SUB_ASSIGN; }
"*="			{ return MUL_ASSIGN; }
"/="			{ return DIV_ASSIGN; }
"%="			{ return MOD_ASSIGN; }
"&="			{ return AND_ASSIGN; }
"^="			{ return XOR_ASSIGN; }
"|="			{ return OR_ASSIGN; }
">>"			{ return RIGHT_OP; }
"<<"			{ return LEFT_OP; }
"++"			{ return INC_OP; }
"--"			{ return DEC_OP; }
"->"			{ return PTR_OP; }
"&&"			{ return AND_OP; }
"||"			{ return OR_OP; }
"<="			{ return LE_OP; }
">="			{ return GE_OP; }
"=="			{ return EQ_OP; }
"!="			{ return NE_OP; }
";"				{ return ';'; }
("{"|"<%")		{ return '{'; }
("}"|"%>")		{ return '}'; }
","				{ return ','; }
":"				{ return ':'; }
"="				{ return '='; }
"("				{ return '('; }
")"				{ return ')'; }
("["|"<:")		{ return '['; }
("]"|":>")		{ return ']'; }
"."				{ return '.'; }
"&"				{ return '&'; }
"!"				{ return '!'; }
"~"				{ return '~'; }
"-"				{ return '-'; }
"+"				{ return '+'; }
"*"				{ return '*'; }
"/"				{ return '/'; }
"%"				{ return '%'; }
"<"				{ return '<'; }
">"				{ return '>'; }
"^"				{ return '^'; }
"|"				{ return '|'; }
"?"				{ return '?'; }

.                 {/* error code. */ return -1;}
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
		if (ret == 0)
			break;
		printf("yylex() return : %d\n", ret);
		printf("#value:%s#\n", yytext);
		printf("--------------\n");
	}
}
#endif
