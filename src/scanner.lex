%{
#include <iostream>
#include <stdio.h>
#include <string>
#include <memory>
#include "ast/ast.h"
#define YYSTYPE std::shared_ptr<ast::Node>
#include "parser.hpp"

// #define DEBUG_LEX
static int ypos = 0;

%}

digit			[0-9]
letter			[a-zA-Z_]
newline			\n
whitespace		[\t ]+
identifier		{letter}+({letter}|{digit})*
string			\"([^\\"\n]|\\.)*\"
comment			\/\*([^*]*\*+[^/*])*([^*]*\*+)\/

num				{digit}+
hex_num			0[xX][a-fA-F0-9]+
float_num		({digit}*\.{digit}+)|({digit}+\.{digit}*)
e_num			{num}e[+]?{num}
e_float			({num}e-{num})|({float_num}e[+-]?{num})

%%

"auto"			{yylval = std::make_shared<ast::Node>("auto"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return AUTO; }
"break"			{yylval = std::make_shared<ast::Node>("break"	, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return BREAK; }
"case"			{yylval = std::make_shared<ast::Node>("case"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CASE; }
"char"			{yylval = std::make_shared<ast::Node>("char"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CHAR; }
"const"			{yylval = std::make_shared<ast::Node>("const"	, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONST; }
"continue"		{yylval = std::make_shared<ast::Node>("continue",	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONTINUE; }
"default"		{yylval = std::make_shared<ast::Node>("default"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return DEFAULT; }
"do"			{yylval = std::make_shared<ast::Node>("do"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return DO; }
"double"		{yylval = std::make_shared<ast::Node>("double"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return DOUBLE; }
"else"			{yylval = std::make_shared<ast::Node>("else"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ELSE; }
"enum"			{yylval = std::make_shared<ast::Node>("enum"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ENUM; }
"extern"		{yylval = std::make_shared<ast::Node>("extern"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return EXTERN; }
"float"			{yylval = std::make_shared<ast::Node>("float"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return FLOAT; }
"for"			{yylval = std::make_shared<ast::Node>("for"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return FOR; }
"goto"			{yylval = std::make_shared<ast::Node>("goto"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return GOTO; }
"if"			{yylval = std::make_shared<ast::Node>("if"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return IF; }
"int"			{yylval = std::make_shared<ast::Node>("int"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return INT; }
"long"			{yylval = std::make_shared<ast::Node>("long"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return LONG; }
"register"		{yylval = std::make_shared<ast::Node>("register",	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return REGISTER; }
"return"		{yylval = std::make_shared<ast::Node>("return"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return RETURN; }
"short"			{yylval = std::make_shared<ast::Node>("short"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return SHORT; }
"signed"		{yylval = std::make_shared<ast::Node>("signed"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return SIGNED; }
"sizeof"		{yylval = std::make_shared<ast::Node>("sizeof"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return SIZEOF; }
"static"		{yylval = std::make_shared<ast::Node>("static"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return STATIC; }
"struct"		{yylval = std::make_shared<ast::Node>("struct"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return STRUCT; }
"switch"		{yylval = std::make_shared<ast::Node>("switch"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return SWITCH; }
"typedef"		{yylval = std::make_shared<ast::Node>("typedef"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return TYPEDEF; }
"union"			{yylval = std::make_shared<ast::Node>("union"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return UNION; }
"unsigned"		{yylval = std::make_shared<ast::Node>("unsigned",	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return UNSIGNED; }
"void"			{yylval = std::make_shared<ast::Node>("void"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return VOID; }
"volatile"		{yylval = std::make_shared<ast::Node>("volatile",	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return VOLATILE; }
"while"			{yylval = std::make_shared<ast::Node>("while"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return WHILE; }

{identifier}	{yylval=std::make_shared<ast::Node>("identifier",	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return IDENTIFIER;}
{comment}		{for (int i = 0; i < yyleng; ++i) 
					if (yytext[i] == '\n') 
						yylineno++, ypos = 0;
					else
						ypos++;
				}
{whitespace}	{ypos += yyleng; }
{newline}		{ypos = 0;	yylineno++;}
{string}		{yylval = std::make_shared<ast::Node>("string"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return STRING_LITERAL; }

{num}			{yylval = std::make_shared<ast::Node>("int"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONSTANT_INT;}
{hex_num}		{yylval = std::make_shared<ast::Node>("int"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONSTANT_INT;}
{float_num}		{yylval = std::make_shared<ast::Node>("float"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONSTANT_FLOAT;}
{e_num}			{yylval = std::make_shared<ast::Node>("int"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONSTANT_INT;}
{e_float}		{yylval = std::make_shared<ast::Node>("float"	,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return CONSTANT_FLOAT;}

"..."			{yylval = std::make_shared<ast::Node>("..."		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ELLIPSIS; }
">>="			{yylval = std::make_shared<ast::Node>(">>="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return RIGHT_ASSIGN; }
"<<="			{yylval = std::make_shared<ast::Node>("<<="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return LEFT_ASSIGN; }
"+="			{yylval = std::make_shared<ast::Node>("+="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ADD_ASSIGN; }
"-="			{yylval = std::make_shared<ast::Node>("-="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return SUB_ASSIGN; }
"*="			{yylval = std::make_shared<ast::Node>("*="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return MUL_ASSIGN; }
"/="			{yylval = std::make_shared<ast::Node>("/="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return DIV_ASSIGN; }
"%="			{yylval = std::make_shared<ast::Node>("%="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return MOD_ASSIGN; }
"&="			{yylval = std::make_shared<ast::Node>("&="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return AND_ASSIGN; }
"^="			{yylval = std::make_shared<ast::Node>("^="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return XOR_ASSIGN; }
"|="			{yylval = std::make_shared<ast::Node>("|="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return OR_ASSIGN; }
">>"			{yylval = std::make_shared<ast::Node>(">>"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return RIGHT_OP; }
"<<"			{yylval = std::make_shared<ast::Node>("<<"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return LEFT_OP; }
"++"			{yylval = std::make_shared<ast::Node>("++"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return INC_OP; }
"--"			{yylval = std::make_shared<ast::Node>("--"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return DEC_OP; }
"->"			{yylval = std::make_shared<ast::Node>("->"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return PTR_OP; }
"&&"			{yylval = std::make_shared<ast::Node>("&&"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return AND_OP; }
"||"			{yylval = std::make_shared<ast::Node>("||"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return OR_OP; }
"<="			{yylval = std::make_shared<ast::Node>("<="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return LE_OP; }
">="			{yylval = std::make_shared<ast::Node>(">="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return GE_OP; }
"=="			{yylval = std::make_shared<ast::Node>("=="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return EQ_OP; }
"!="			{yylval = std::make_shared<ast::Node>("!="		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return NE_OP; }
";"				{yylval = std::make_shared<ast::Node>(";"		,	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ';'; }
("{"|"<%")		{yylval = std::make_shared<ast::Node>("{"		, 	"{",	yylineno, ypos, yylineno, ypos+=yyleng); return '{'; }
("}"|"%>")		{yylval = std::make_shared<ast::Node>("}"		, 	"}",	yylineno, ypos, yylineno, ypos+=yyleng); return '}'; }
","				{yylval = std::make_shared<ast::Node>(","		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ','; }
":"				{yylval = std::make_shared<ast::Node>(":"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ':'; }
"="				{yylval = std::make_shared<ast::Node>("="		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '='; }
"("				{yylval = std::make_shared<ast::Node>("("		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '('; }
")"				{yylval = std::make_shared<ast::Node>(")"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return ')'; }
("["|"<:")		{yylval = std::make_shared<ast::Node>("["		, 	"[",	yylineno, ypos, yylineno, ypos+=yyleng); return '['; }
("]"|":>")		{yylval = std::make_shared<ast::Node>("]"		, 	"]",	yylineno, ypos, yylineno, ypos+=yyleng); return ']'; }
"."				{yylval = std::make_shared<ast::Node>("."		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '.'; }
"&"				{yylval = std::make_shared<ast::Node>("&"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '&'; }
"!"				{yylval = std::make_shared<ast::Node>("!"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '!'; }
"~"				{yylval = std::make_shared<ast::Node>("~"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '~'; }
"-"				{yylval = std::make_shared<ast::Node>("-"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '-'; }
"+"				{yylval = std::make_shared<ast::Node>("+"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '+'; }
"*"				{yylval = std::make_shared<ast::Node>("*"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '*'; }
"/"				{yylval = std::make_shared<ast::Node>("/"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '/'; }
"%"				{yylval = std::make_shared<ast::Node>("%"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '%'; }
"<"				{yylval = std::make_shared<ast::Node>("<"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '<'; }
">"				{yylval = std::make_shared<ast::Node>(">"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '>'; }
"^"				{yylval = std::make_shared<ast::Node>("^"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '^'; }
"|"				{yylval = std::make_shared<ast::Node>("|"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '|'; }
"?"				{yylval = std::make_shared<ast::Node>("?"		, 	yytext, yylineno, ypos, yylineno, ypos+=yyleng); return '?'; }

.				{ypos += yyleng; /* error code. */ return -1;}
%%

int yywrap()
{
	return(1);
}

#ifdef DEBUG_LEX
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
