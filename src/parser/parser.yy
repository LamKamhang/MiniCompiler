%{
#include <stdio.h>
#include <memory>
#include "../ast/ast.h"
#include "../lib/json/json.h"

#define YYSTYPE std::shared_ptr<ast::Node>
extern int yylex(void);
void yyerror(const char *);

YYSTYPE root;

%}

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_SHIFT_OP RIGHT_SHIFT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_SHIFT_ASSIGN RIGHT_SHIFT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%nonassoc IFX 
%nonassoc ELSE

%start program
%%

program
: translation_unit	{
  // Json::StyledStreamWriter writer(" ");
  // writer.write(std::cout, ast::exports($1));
  root = $1;
 }
;

primary_expression
: IDENTIFIER	{
  $$ = $1;
 }
| CONSTANT	{
  $$ = $1;
 }
| STRING_LITERAL	{
  $$ = $1;
 }
| '(' expression ')'	{
  $$ = std::make_shared<ast::Node>("primary_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($2);
 }
;

postfix_expression
: primary_expression	{
  $$ = $1;
 }
| postfix_expression '[' expression ']'	{
	$$ = std::make_shared<ast::Node>("index_reference", $1->get_left(), $4->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
	$$->children.emplace_back($4);
}
| postfix_expression '(' ')'	{
	$$ = std::make_shared<ast::Node>("function_call", $1->get_left(), $3->get_right());
  auto a_list = std::make_shared<ast::Node>("argument_list", $2->get_left(), $3->get_right());
	$$->children.emplace_back($1);
  $$->children.emplace_back(a_list);
}
| postfix_expression '(' argument_expression_list ')'	{
	$$ = std::make_shared<ast::Node>("function_call", $1->get_left(), $4->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($3);
}
| postfix_expression '.' IDENTIFIER	{
	$$ = std::make_shared<ast::Node>("member_reference", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
}
| postfix_expression PTR_OP IDENTIFIER	{
	$$ = std::make_shared<ast::Node>("pointer_reference", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
}
| postfix_expression INC_OP	{
	$$ = std::make_shared<ast::Node>("post_inc_expression", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
| postfix_expression DEC_OP	{
	$$ = std::make_shared<ast::Node>("post_dev_expression", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
;

argument_expression_list
: assignment_expression	{
  $$ = std::make_shared<ast::Node>("argument_expression_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| argument_expression_list ',' assignment_expression	{
  $$ = $1;
  $$->set_right($3->get_right());
  $$->children.emplace_back($3);
  }
;

unary_expression
: postfix_expression	{
  $$ = $1;
 }
| INC_OP unary_expression	{
  $$ = std::make_shared<ast::Node>("pre_inc_operator", $1->get_left(), $2->get_right());
  $$->children.emplace_back($2);
 }
| DEC_OP unary_expression	{
  $$ = std::make_shared<ast::Node>("pre_dec_operator", $1->get_left(), $2->get_right());
  $$->children.emplace_back($2);
 }
| unary_operator cast_expression	{
  $$ = std::make_shared<ast::Node>("unary_operator", $1->get_left(), $2->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
 }
| SIZEOF unary_expression	{
  $$ = std::make_shared<ast::Node>("sizeof_operator", $1->get_left(), $2->get_right());
  $$->children.emplace_back($2);
 }
| SIZEOF '(' type_name ')'	{
  $$ = std::make_shared<ast::Node>("sizeof_operator", $1->get_left(), $4->get_right());
  $$->children.emplace_back($3);
  }
;

unary_operator
: '&'	{
  $$ = $1;
  $$->type = "unary_operator";
 }
| '*'	{
  $$ = $1;
  $$->type = "unary_operator";
  }
| '+'	{
  $$ = $1;
  $$->type = "unary_operator";
  }
| '-'	{
  $$ = $1;
  $$->type = "unary_operator";
  }
| '~'	{
  $$ = $1;
  $$->type = "unary_operator";
  }
| '!'	{
  $$ = $1;
  $$->type = "unary_operator";
  }
;

cast_expression
: unary_expression	{
  $$ = $1;
 }
| '(' type_name ')' cast_expression	{
  $$ = std::make_shared<ast::Node>("cast_expression", $2->get_left(), $4->get_right());
  $$->children.emplace_back($2);
  $$->children.emplace_back($4);
  }
;

multiplicative_expression
: cast_expression	{
  $$ = $1;
 }
| multiplicative_expression '*' cast_expression	{
  $$ = std::make_shared<ast::Node>("mul_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
| multiplicative_expression '/' cast_expression	{
  $$ = std::make_shared<ast::Node>("div_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
| multiplicative_expression '%' cast_expression	{
  $$ = std::make_shared<ast::Node>("mod_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
;

additive_expression
: multiplicative_expression	{
  $$ = $1;
 }
| additive_expression '+' multiplicative_expression	{
  $$ = std::make_shared<ast::Node>("add_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
| additive_expression '-' multiplicative_expression	{
  $$ = std::make_shared<ast::Node>("sub_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
;

shift_expression
: additive_expression	{
  $$ = $1;
 }
| shift_expression LEFT_SHIFT_OP additive_expression		{
  $$ = std::make_shared<ast::Node>("left_shift_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
| shift_expression RIGHT_SHIFT_OP additive_expression	{
  $$ = std::make_shared<ast::Node>("right_shift_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
;

relational_expression
: shift_expression	{
  $$ = $1;
 }
| relational_expression '<' shift_expression	{
  $$ = std::make_shared<ast::Node>("lt_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
| relational_expression '>' shift_expression	{
  $$ = std::make_shared<ast::Node>("gt_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
| relational_expression LE_OP shift_expression	{
  $$ = std::make_shared<ast::Node>("le_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
| relational_expression GE_OP shift_expression	{
  $$ = std::make_shared<ast::Node>("ge_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
;

equality_expression
: relational_expression	{
  $$ = $1;
 }
| equality_expression EQ_OP relational_expression	{
  $$ = std::make_shared<ast::Node>("equality_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
| equality_expression NE_OP relational_expression	{
  $$ = std::make_shared<ast::Node>("not_equality_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
;

and_expression
: equality_expression	{
  $$ = $1;
 }
| and_expression '&' equality_expression	{
  $$ = std::make_shared<ast::Node>("and_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
;

exclusive_or_expression
: and_expression		{
  $$ = $1;
 }
| exclusive_or_expression '^' and_expression	{
  $$ = std::make_shared<ast::Node>("exclusive_or_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
;

inclusive_or_expression
: exclusive_or_expression	{
  $$ = $1;
 }
| inclusive_or_expression '|' exclusive_or_expression	{
  $$ = std::make_shared<ast::Node>("inclusive_or_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  }
;

logical_and_expression
: inclusive_or_expression	{
  $$ = $1;
 }
| logical_and_expression AND_OP inclusive_or_expression	{
  $$ = std::make_shared<ast::Node>("logical_and_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
;

logical_or_expression
: logical_and_expression	{
  $$ = $1;
 }
| logical_or_expression OR_OP logical_and_expression	{
  $$ = std::make_shared<ast::Node>("logical_or_expression", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
;

conditional_expression
: logical_or_expression	{
  $$ = $1;
 }
| logical_or_expression '?' expression ':' conditional_expression	{
  $$ = std::make_shared<ast::Node>("conditional_expression", $1->get_left(), $5->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
  $$->children.emplace_back($5);
 }
;

assignment_expression
: conditional_expression	{
  $$ = $1;
 }
| unary_expression assignment_operator assignment_expression	{
  $$ = std::make_shared<ast::Node>($2->type, $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
;

assignment_operator
: '='	{
  $$ = $1;
  $$->type = "assign_expr";
 }
| MUL_ASSIGN	{
   $$ = $1;
   $$->type = "mul_assign_expr";
  }
| DIV_ASSIGN	{
   $$ = $1;
   $$->type = "div_assign_expr";
  }
| MOD_ASSIGN	{
   $$ = $1;
   $$->type = "mod_assign_expr";
  }
| ADD_ASSIGN	{
   $$ = $1;
   $$->type = "add_assign_expr";
  }
| SUB_ASSIGN	{
   $$ = $1;
   $$->type = "sub_assign_expr";
  }
| LEFT_SHIFT_ASSIGN	{
   $$ = $1;
   $$->type = "left_shift_assign_expr";
  }
| RIGHT_SHIFT_ASSIGN	{
   $$ = $1;
   $$->type = "right_shift_assign_expr";
  }
| AND_ASSIGN	{
   $$ = $1;
   $$->type = "and_assign_expr";
  }
| XOR_ASSIGN	{
   $$ = $1;
   $$->type = "xor_assign_expr";
  }
| OR_ASSIGN	{
   $$ = $1;
   $$->type = "or_assign_expr";
  }
;

expression
: assignment_expression	{
  $$ = std::make_shared<ast::Node>("expression", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| expression ',' assignment_expression	{
  if ($1->type == "comma_expression")
  {
    $$ = $1;
    $$->set_right($3->get_right());
    $$->children.emplace_back($3);
  }
  else
  {
    $$ = std::make_shared<ast::Node>("comma_expression", $1->get_left(), $3->get_right());
    $$->children.emplace_back($1);
    $$->children.emplace_back($3);
  }
 }
;

constant_expression
: conditional_expression	{
	$$ = $1;
}
;

declaration
: declaration_specifiers ';'	{
	$$ = std::make_shared<ast::Node>("declaration", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
}
| declaration_specifiers init_declarator_list ';'	{
	$$ = std::make_shared<ast::Node>("declaration", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
;

declaration_specifiers
: storage_class_specifier	{
  $$ = std::make_shared<ast::Node>("declaration_specifiers", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| storage_class_specifier declaration_specifiers	{
  $$ = std::make_shared<ast::Node>("declaration_specifiers", $1->get_left(), $2->get_right());
  $$->children.emplace_back($1);
  for (auto & child : $2->children)
  {
    $$->children.emplace_back(child);
  }
 }
| type_specifier	{
  $$ = std::make_shared<ast::Node>("declaration_specifiers", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| type_specifier declaration_specifiers	{
  $$ = std::make_shared<ast::Node>("declaration_specifiers", $1->get_left(), $2->get_right());
  $$->children.emplace_back($1);
  for (auto & child : $2->children)
  {
    $$->children.emplace_back(child);
  }
 }
| type_qualifier	{
  $$ = std::make_shared<ast::Node>("declaration_specifiers", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| type_qualifier declaration_specifiers	{
  $$ = std::make_shared<ast::Node>("declaration_specifiers", $1->get_left(), $2->get_right());
  $$->children.emplace_back($1);
  for (auto & child : $2->children)
  {
    $$->children.emplace_back(child);
  }
 }
;

init_declarator_list
: init_declarator	{
	$$ = std::make_shared<ast::Node>("init_declarator_list", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| init_declarator_list ',' init_declarator	{
	$$ = $1;
	$$->set_right($3->get_right());
	$$->children.emplace_back($3);
}
;

init_declarator
: declarator	{
	$$ = $1;
}
| declarator '=' initializer	{
	$$ = std::make_shared<ast::Node>("init_declarator", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($3);
}
;

storage_class_specifier
: TYPEDEF	{
	$$ = $1;
	$$->type = "storage_class_specifier";
}
| EXTERN	{
	$$ = $1;
	$$->type = "storage_class_specifier";
}
| STATIC	{
	$$ = $1;
	$$->type = "storage_class_specifier";
}
| AUTO	{
	$$ = $1;
	$$->type = "storage_class_specifier";
}
| REGISTER	{
	$$ = $1;
	$$->type = "storage_class_specifier";
}
;

type_specifier
: VOID	{
  $$ = $1;
  $$->type = "type_specifier";
 }
| CHAR	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| SHORT	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| INT	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| LONG	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| FLOAT	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| DOUBLE	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| SIGNED	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| UNSIGNED	{
  $$ = $1;
  $$->type = "type_specifier";
  }
| struct_or_union_specifier	{
  $$ = $1;
  }
| enum_specifier	{
  $$ = $1;
  }
;

struct_or_union_specifier
: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
	$$ = std::make_shared<ast::Node>("struct_or_union_specifier", $1->get_left(), $5->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($4);
}
| struct_or_union '{' struct_declaration_list '}'	{
	$$ = std::make_shared<ast::Node>("struct_or_union_specifier", $1->get_left(), $4->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($3);
}
| struct_or_union IDENTIFIER	{
	$$ = std::make_shared<ast::Node>("struct_or_union_specifier", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
;

struct_or_union
: STRUCT	{
	$$ = $1;
}
| UNION	{
	$$ = $1;
}
;

struct_declaration_list
: struct_declaration	{
	$$ = std::make_shared<ast::Node>("struct_declaration_list", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| struct_declaration_list struct_declaration	{
	$$ = $1;
	$$->set_right($2->get_right());
	$$->children.emplace_back($2);
}
;

struct_declaration
: specifier_qualifier_list struct_declarator_list ';'	{
	$$ = std::make_shared<ast::Node>("struct_declaration", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
;

specifier_qualifier_list
: specifier_qualifier_list type_specifier	{
	$$ = $1;
  $1->children.emplace_back($2);
}
| type_specifier	{
  $$ = std::make_shared<ast::Node>("specifier_qualifier_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
}
| specifier_qualifier_list type_qualifier	{
	$$ = $1;
  $1->children.emplace_back($2);
}
| type_qualifier	{
  $$ = std::make_shared<ast::Node>("specifier_qualifier_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
}
;

struct_declarator_list
: struct_declarator	{
	$$ = std::make_shared<ast::Node>("struct_declarator_list", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| struct_declarator_list ',' struct_declarator	{
	$$ = $1;
	$$->children.emplace_back($3);
}
;

struct_declarator
: declarator	{
	$$ = std::make_shared<ast::Node>("struct_declarator", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| ':' constant_expression	{
	$$ = std::make_shared<ast::Node>("struct_declarator", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
| declarator ':' constant_expression	{
	$$ = std::make_shared<ast::Node>("struct_declarator", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
}
;

enum_specifier
: ENUM '{' enumerator_list '}'	{
	$$ = std::make_shared<ast::Node>("enum_specifier", $1->get_left(), $4->get_right());
	$$->children.emplace_back($3);
}
| ENUM IDENTIFIER '{' enumerator_list '}'	{
	$$ = std::make_shared<ast::Node>("enum_specifier", $1->get_left(), $5->get_right());
	$$->children.emplace_back($2);
	$$->children.emplace_back($4);
}
| ENUM IDENTIFIER	{
	$$ = std::make_shared<ast::Node>("enum_specifier", $1->get_left(), $2->get_right());
	$$->children.emplace_back($2);
}
;

enumerator_list
: enumerator	{
	$$ = std::make_shared<ast::Node>("enumerator_list", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| enumerator_list ',' enumerator	{
	$$ = $1;
	$$->set_right($3->get_right());
	$$->children.emplace_back($3);
}
;

enumerator
: IDENTIFIER	{
	$$ = std::make_shared<ast::Node>("enumerator", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| IDENTIFIER '=' constant_expression	{
	$$ = std::make_shared<ast::Node>("enumerator", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($3);
}
;

type_qualifier
: CONST	{
	$$ = $1;
	$$->type = "type_qualifier";
}
| VOLATILE	{
	$$ = $1;
	$$->type = "type_qualifier";
}
;

declarator
: pointer direct_declarator	{
  $$ = std::make_shared<ast::Node>("declarator", $1->get_left(), $2->get_right());
  for (auto & child : $1->children)
  {
      $$->children.emplace_back(child);
  }
  for (auto & child : $2->children)
  {
      $$->children.emplace_back(child);
  }
 }
| direct_declarator	{
  $$ = std::make_shared<ast::Node>("declarator", $1->get_left(), $1->get_right());
  for (auto & child : $1->children)
  {
      $$->children.emplace_back(child);
  }
 }
;

direct_declarator
: IDENTIFIER	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| '(' declarator ')'	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
 }
| direct_declarator '[' constant_expression ']'	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $4->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
  $$->children.emplace_back($4);
 }
| direct_declarator '[' ']'	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
 }
| direct_declarator '(' parameter_list ')'	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $4->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($3);
 }
| direct_declarator '(' identifier_list ')'	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $4->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
  $$->children.emplace_back($4);
 }
| direct_declarator '(' ')'	{
  $$ = std::make_shared<ast::Node>("direct_declarator", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
 }
;

pointer
: '*'	{
	$$ = std::make_shared<ast::Node>("pointer", $1->get_left(), $1->get_right());
	$$->children.emplace_back($1);
}
| '*' type_qualifier_list	{
	$$ = std::make_shared<ast::Node>("pointer", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
  for (auto &child : $2->children)
  {
    $$->children.emplace_back(child);
  }
}
| '*' pointer	{
	$$ = std::make_shared<ast::Node>("pointer", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	for (auto &child : $2->children)
  {
    $$->children.emplace_back(child);
  }
}
| '*' type_qualifier_list pointer	{
	$$ = std::make_shared<ast::Node>("pointer", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
  for (auto &child : $2->children)
  {
    $$->children.emplace_back(child);
  }
  for (auto &child : $3->children)
  {
    $$->children.emplace_back(child);
  }
}
;

type_qualifier_list
: type_qualifier {
  $$ = std::make_shared<ast::Node>("type_qualifier_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| type_qualifier_list type_qualifier {
  $$ = $1;
  $$->children.emplace_back($2);
}
;

parameter_list
: parameter_declaration	{
  $$ = std::make_shared<ast::Node>("parameter_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| parameter_list ',' parameter_declaration {
  $$ = $1;
  $$->children.emplace_back($3);
}
;

parameter_declaration
: declaration_specifiers declarator	{
  $$ = std::make_shared<ast::Node>("parameter_declaration", $1->get_left(), $1->get_right());
  for (auto & child : $1->children)
  {
      $$->children.emplace_back(child);
  }
  for (auto & child : $2->children)
  {
      $$->children.emplace_back(child);
  }
 }
| declaration_specifiers abstract_declarator	{
  $$ = std::make_shared<ast::Node>("parameter_declaration", $1->get_left(), $1->get_right());
  for (auto & child : $1->children)
  {
      $$->children.emplace_back(child);
  }
  for (auto & child : $2->children)
  {
      $$->children.emplace_back(child);
  }
 }
| declaration_specifiers	{
  $$ = std::make_shared<ast::Node>("parameter_declaration", $1->get_left(), $1->get_right());
  for (auto & child : $1->children)
  {
      $$->children.emplace_back(child);
  }
 }
;

identifier_list
: IDENTIFIER	{
  $$ = std::make_shared<ast::Node>("identifier_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| identifier_list ',' IDENTIFIER	{
  $$ = $1;
  $$->children.emplace_back($3);
  }
;

type_name
: specifier_qualifier_list	{
  $$ = $1;
  $$->type = "type_name";
}
| specifier_qualifier_list abstract_declarator	{
  $$ = $1;
  $$->type = "type_name";
  for (auto & child : $2->children)
  {
    $$->children.emplace_back(child);
  }
}
;

abstract_declarator
: pointer	{
	$$ = $1;
}
| direct_abstract_declarator	{
	$$ = $1;
}
| pointer direct_abstract_declarator	{
	$$ = std::make_shared<ast::Node>("abstract_declarator", $1->get_left(), $2->get_right());
	for (auto & child : $1->children)
  {
    $$->children.emplace_back(child);
  }
  for (auto & child : $2->children)
  {
    $$->children.emplace_back(child);
  }
}
;

direct_abstract_declarator
: '(' abstract_declarator ')'	{
	$$ = std::make_shared<ast::Node>("direct_abstract_declarator", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
  for (auto & child : $2->children)
  {
    $$->children.emplace_back(child);
  }
	$$->children.emplace_back($3);
}
| '[' ']'	{
	$$ = std::make_shared<ast::Node>("direct_abstract_declarator", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
| '[' constant_expression ']'	{
	$$ = std::make_shared<ast::Node>("direct_abstract_declarator", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
}
| direct_abstract_declarator '[' ']'	{
	$$ = $1;
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
}
| direct_abstract_declarator '[' constant_expression ']'	{
	$$ = $1;
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
	$$->children.emplace_back($4);
}
| '(' ')'	{
	$$ = std::make_shared<ast::Node>("direct_abstract_declarator", $1->get_left(), $2->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
}
| '(' parameter_list ')'	{
	$$ = std::make_shared<ast::Node>("direct_abstract_declarator", $1->get_left(), $3->get_right());
	$$->children.emplace_back($1);
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
}
| direct_abstract_declarator '(' ')'	{
	$$ = $1;
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
}
| direct_abstract_declarator '(' parameter_list ')'	{
	$$ = $1;
	$$->children.emplace_back($2);
	$$->children.emplace_back($3);
	$$->children.emplace_back($4);
}
;

initializer
: assignment_expression	{
  $$ = std::make_shared<ast::Node>("expression", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| '{' initializer_list '}'	{
  $$ = $2;
  }
| '{' initializer_list ',' '}'	{
  $$ = $2;
  }
;

initializer_list
: initializer	{
  $$ = std::make_shared<ast::Node>("initializer_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| initializer_list ',' initializer	{
  $$ = $1;
  $1->children.emplace_back($3);
  }
;

statement
: labeled_statement	{
  $$ = $1;
 }
| compound_statement	{
  $$ = $1;
  }
| expression_statement	{
  $$ = $1;
  }
| selection_statement	{
  $$ = $1;
  }
| iteration_statement	{
  $$ = $1;
  }
| jump_statement	{
  $$ = $1;
  }
;

labeled_statement
: CASE constant_expression ':' statement	{
  $$ = std::make_shared<ast::Node>("case_statement", $1->get_left(), $4->get_right());
  $$->children.emplace_back($2);
  $$->children.emplace_back($4);
 }
| DEFAULT ':' statement	{
  $$ = std::make_shared<ast::Node>("default_statement", $1->get_left(), $3->get_right());
  $$->children.emplace_back($3);
  }
;

compound_statement
: '{' '}'	{
  $$ = std::make_shared<ast::Node>("compound_statement", $1->get_left(), $2->get_right());
 }
| '{' statement_list '}'	{
  $$ = std::make_shared<ast::Node>("compound_statement", $1->get_left(), $3->get_right());
  $$->children.emplace_back($2);
  }
| '{' declaration_list '}'	{
  $$ = std::make_shared<ast::Node>("compound_statement", $1->get_left(), $3->get_right());
  $$->children.emplace_back($2);
  }
| '{' declaration_list statement_list '}'	{
  $$ = std::make_shared<ast::Node>("compound_statement", $1->get_left(), $4->get_right());
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
  }
;

declaration_list
: declaration	{
  $$ = std::make_shared<ast::Node>("declaration_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| declaration_list declaration	{
  $$ = $1;
  $$->children.emplace_back($2);
 }
;

statement_list
: statement	{
  $$ = std::make_shared<ast::Node>("statement_list", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| statement_list statement	{
  $$ = $1;
  $1->children.emplace_back($2);
 }
;

expression_statement
: ';'	{
  $$ = std::make_shared<ast::Node>("expression_statement", $1->get_left(), $1->get_right());
 }
| expression ';'	{
  $$ = $1;
  }
;

selection_statement
: IF '(' expression ')' statement %prec IFX	{
  $$ = std::make_shared<ast::Node>("if_statement", $1->get_left(), $5->get_right());
  $$->children.emplace_back($3);
  $$->children.emplace_back($5);
 }
| IF '(' expression ')' statement ELSE statement	{
  $$ = std::make_shared<ast::Node>("if_else_statement", $1->get_left(), $7->get_right());
  $$->children.emplace_back($3);
  $$->children.emplace_back($5);
  $$->children.emplace_back($7);
  }
| SWITCH '(' expression ')' statement {
  $$ = std::make_shared<ast::Node>("switch_statement", $1->get_left(), $5->get_right());
  $$->children.emplace_back($3);
  $$->children.emplace_back($5);
}
;

iteration_statement
: WHILE '(' expression ')' statement	{
  $$ = std::make_shared<ast::Node>("while_statement", $1->get_left(), $5->get_right());
  $$->children.emplace_back($3);
  $$->children.emplace_back($5);
 }
| DO statement WHILE '(' expression ')' ';'	{
  $$ = std::make_shared<ast::Node>("do_statement", $1->get_left(), $7->get_right());
  $$->children.emplace_back($2);
  $$->children.emplace_back($5);
 }
| FOR '(' expression_statement expression_statement ')' statement	{
  $$ = std::make_shared<ast::Node>("for_statement", $1->get_left(), $6->get_right());
  $$->children.emplace_back($3);
  $$->children.emplace_back($4);
  $$->children.emplace_back($6);
  }
| FOR '(' expression_statement expression_statement expression ')' statement	{
  $$ = std::make_shared<ast::Node>("for_statement", $1->get_left(), $7->get_right());
  $$->children.emplace_back($3);
  $$->children.emplace_back($4);
  $$->children.emplace_back($5);
  $$->children.emplace_back($7);
  }
;

jump_statement
: CONTINUE ';'	{
  $$ = $1;
 }
| BREAK ';'	{
  $$ = $1;
  }
| RETURN ';'	{
  $$ = $1;
  $$->type = "return_only";
  }
| RETURN expression ';'	{
  $$ = std::make_shared<ast::Node>("return_expr", $1->get_left(), $3->get_right());
  $$->children.emplace_back($2);
 }
;

translation_unit
: external_declaration	{
  $$ = std::make_shared<ast::Node>("translation_unit", $1->get_left(), $1->get_right());
  $$->children.emplace_back($1);
 }
| translation_unit external_declaration	{
  $$ = $1;
  $$->set_right($2->get_right());
  $$->children.emplace_back($2);
 }
;

external_declaration
: function_definition	{
  $$ = $1;
 }
| declaration	{
  $$ = $1;
  }
;

function_definition
: declaration_specifiers declarator compound_statement	{
  $$ = std::make_shared<ast::Node>("function_definition", $1->get_left(), $3->get_right());
  $$->children.emplace_back($1);
  $$->children.emplace_back($2);
  $$->children.emplace_back($3);
 }
;
%%

void yyerror(const char *s)
{
  printf("%s\n", s);
}
