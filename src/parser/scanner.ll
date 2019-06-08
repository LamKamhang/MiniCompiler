%{
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <memory>
#include "../ast/ast.h"

#define YYSTYPE std::shared_ptr<ast::Node>
#include "parser.hh"

  // #define DEBUG_LEX
static int ypos = 0;

inline std::string hex2num();
inline std::string e2float();

%}

digit			[0-9]
letter			[a-zA-Z_]
newline			\n
whitespace		[\t ]+
identifier		{letter}+({letter}|{digit})*
string			\"([^\\"\n]|\\.)*\"
comment			\/\*([^*]*\*+[^/*])*([^*]*\*+)\/

char			\'(\\.|[^\\'])+\'
num				{digit}+
hex_num			0[xX][a-fA-F0-9]+
float_num		({digit}*\.{digit}+)|({digit}+\.{digit}*)
e_float			({num}|{float_num})[eE][+-]?{num}

%%

"auto"			{yylval = std::make_shared<ast::Node>("auto"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return AUTO; }
"break"			{yylval = std::make_shared<ast::Node>("break"	, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return BREAK; }
"case"			{yylval = std::make_shared<ast::Node>("case"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CASE; }
"char"			{yylval = std::make_shared<ast::Node>("char"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CHAR; }
"const"			{yylval = std::make_shared<ast::Node>("const"	, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONST; }
"continue"		{yylval = std::make_shared<ast::Node>("continue",	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONTINUE; }
"default"		{yylval = std::make_shared<ast::Node>("default"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return DEFAULT; }
"do"			{yylval = std::make_shared<ast::Node>("do"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return DO; }
"double"		{yylval = std::make_shared<ast::Node>("double"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return DOUBLE; }
"else"			{yylval = std::make_shared<ast::Node>("else"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ELSE; }
"enum"			{yylval = std::make_shared<ast::Node>("enum"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ENUM; }
"extern"		{yylval = std::make_shared<ast::Node>("extern"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return EXTERN; }
"float"			{yylval = std::make_shared<ast::Node>("float"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return FLOAT; }
"for"			{yylval = std::make_shared<ast::Node>("for"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return FOR; }
"goto"			{yylval = std::make_shared<ast::Node>("goto"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return GOTO; }
"if"			{yylval = std::make_shared<ast::Node>("if"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return IF; }
"int"			{yylval = std::make_shared<ast::Node>("int"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return INT; }
"long"			{yylval = std::make_shared<ast::Node>("long"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return LONG; }
"register"		{yylval = std::make_shared<ast::Node>("register",	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return REGISTER; }
"return"		{yylval = std::make_shared<ast::Node>("return"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return RETURN; }
"short"			{yylval = std::make_shared<ast::Node>("short"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return SHORT; }
"signed"		{yylval = std::make_shared<ast::Node>("signed"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return SIGNED; }
"sizeof"		{yylval = std::make_shared<ast::Node>("sizeof"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return SIZEOF; }
"static"		{yylval = std::make_shared<ast::Node>("static"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return STATIC; }
"struct"		{yylval = std::make_shared<ast::Node>("struct"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return STRUCT; }
"switch"		{yylval = std::make_shared<ast::Node>("switch"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return SWITCH; }
"typedef"		{yylval = std::make_shared<ast::Node>("typedef"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return TYPEDEF; }
"union"			{yylval = std::make_shared<ast::Node>("union"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return UNION; }
"unsigned"		{yylval = std::make_shared<ast::Node>("unsigned",	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return UNSIGNED; }
"void"			{yylval = std::make_shared<ast::Node>("void"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return VOID; }
"volatile"		{yylval = std::make_shared<ast::Node>("volatile",	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return VOLATILE; }
"while"			{yylval = std::make_shared<ast::Node>("while"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return WHILE; }

{identifier}	{yylval=std::make_shared<ast::Node>("identifier",	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return IDENTIFIER;}
{comment}		{for (int i = 0; i < yyleng; ++i) 
					if (yytext[i] == '\n') 
						yylineno++, ypos = 0;
					else
						ypos++;
				}
{whitespace}	{ypos += yyleng; }
{newline}		{ypos = 0;	yylineno++;}
{string}		{yylval = std::make_shared<ast::Node>("string"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return STRING_LITERAL; }
{char}         	{yylval = std::make_shared<ast::Node>("char"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONSTANT;}

{num}			{yylval = std::make_shared<ast::Node>("int"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONSTANT;}
{hex_num}		{yylval = std::make_shared<ast::Node>("int"		,hex2num(), yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONSTANT;}
{float_num}		{yylval = std::make_shared<ast::Node>("float"	,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONSTANT;}
{e_float}		{yylval = std::make_shared<ast::Node>("float"	,e2float(), yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return CONSTANT;}

"..."			{yylval = std::make_shared<ast::Node>("..."		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ELLIPSIS; }
">>="			{yylval = std::make_shared<ast::Node>(">>="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return RIGHT_SHIFT_ASSIGN; }
"<<="			{yylval = std::make_shared<ast::Node>("<<="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return LEFT_SHIFT_ASSIGN; }
"+="			{yylval = std::make_shared<ast::Node>("+="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ADD_ASSIGN; }
"-="			{yylval = std::make_shared<ast::Node>("-="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return SUB_ASSIGN; }
"*="			{yylval = std::make_shared<ast::Node>("*="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return MUL_ASSIGN; }
"/="			{yylval = std::make_shared<ast::Node>("/="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return DIV_ASSIGN; }
"%="			{yylval = std::make_shared<ast::Node>("%="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return MOD_ASSIGN; }
"&="			{yylval = std::make_shared<ast::Node>("&="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return AND_ASSIGN; }
"^="			{yylval = std::make_shared<ast::Node>("^="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return XOR_ASSIGN; }
"|="			{yylval = std::make_shared<ast::Node>("|="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return OR_ASSIGN; }
">>"			{yylval = std::make_shared<ast::Node>(">>"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return RIGHT_SHIFT_OP; }
"<<"			{yylval = std::make_shared<ast::Node>("<<"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return LEFT_SHIFT_OP; }
"++"			{yylval = std::make_shared<ast::Node>("++"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return INC_OP; }
"--"			{yylval = std::make_shared<ast::Node>("--"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return DEC_OP; }
"->"			{yylval = std::make_shared<ast::Node>("->"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return PTR_OP; }
"&&"			{yylval = std::make_shared<ast::Node>("&&"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return AND_OP; }
"||"			{yylval = std::make_shared<ast::Node>("||"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return OR_OP; }
"<="			{yylval = std::make_shared<ast::Node>("<="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return LE_OP; }
">="			{yylval = std::make_shared<ast::Node>(">="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return GE_OP; }
"=="			{yylval = std::make_shared<ast::Node>("=="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return EQ_OP; }
"!="			{yylval = std::make_shared<ast::Node>("!="		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return NE_OP; }
";"				{yylval = std::make_shared<ast::Node>(";"		,	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ';'; }
("{"|"<%")		{yylval = std::make_shared<ast::Node>("{"		, 	"{",	yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '{'; }
("}"|"%>")		{yylval = std::make_shared<ast::Node>("}"		, 	"}",	yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '}'; }
","				{yylval = std::make_shared<ast::Node>(","		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ','; }
":"				{yylval = std::make_shared<ast::Node>(":"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ':'; }
"="				{yylval = std::make_shared<ast::Node>("="		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '='; }
"("				{yylval = std::make_shared<ast::Node>("("		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '('; }
")"				{yylval = std::make_shared<ast::Node>(")"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ')'; }
("["|"<:")		{yylval = std::make_shared<ast::Node>("["		, 	"[",	yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '['; }
("]"|":>")		{yylval = std::make_shared<ast::Node>("]"		, 	"]",	yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return ']'; }
"."				{yylval = std::make_shared<ast::Node>("."		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '.'; }
"&"				{yylval = std::make_shared<ast::Node>("&"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '&'; }
"!"				{yylval = std::make_shared<ast::Node>("!"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '!'; }
"~"				{yylval = std::make_shared<ast::Node>("~"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '~'; }
"-"				{yylval = std::make_shared<ast::Node>("-"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '-'; }
"+"				{yylval = std::make_shared<ast::Node>("+"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '+'; }
"*"				{yylval = std::make_shared<ast::Node>("*"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '*'; }
"/"				{yylval = std::make_shared<ast::Node>("/"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '/'; }
"%"				{yylval = std::make_shared<ast::Node>("%"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '%'; }
"<"				{yylval = std::make_shared<ast::Node>("<"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '<'; }
">"				{yylval = std::make_shared<ast::Node>(">"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '>'; }
"^"				{yylval = std::make_shared<ast::Node>("^"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '^'; }
"|"				{yylval = std::make_shared<ast::Node>("|"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '|'; }
"?"				{yylval = std::make_shared<ast::Node>("?"		, 	yytext, yylineno, ypos, yylineno, ypos+yyleng); ypos+=yyleng; return '?'; }

.				{ypos += yyleng; /* error code. */ return -1;}
%%

int yywrap()
{
	return(1);
}

inline std::string hex2num()
{
	unsigned long res = 0;
	char c;
	for (int i = 2; i < yyleng; ++i)
	{
		c = yytext[i];
		res = res * 16 + (c <= '9' ? (c - '0')
								   : (c & 0xdf) - 'A' + 10);
	}
	return std::to_string(res);
}

inline std::string e2float()
{
	double head = 0;
	long exp = 0;
	for (int i = 0; i < yyleng; ++i)
	{
		if ((yytext[i] & 0xdf) == 'E')
		{
			head = std::stod(std::string(yytext).substr(0, i));
			exp = std::stoi(std::string(yytext).substr(i+1));
			break;
		}
	}
	return std::to_string(pow(10, exp)*head);
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
