// 宏定义
%{
  #define YYSTYPE double
  #include <math.h>

  #include <stdio.h>
  int yylex (void);
  void yyerror (char const *);
%}

// 定义符号与运算符优先级
%token NUM    //数字
%left '-' '+' // 加减->左结合
%left '*' '/' // 乘除->左结合
%left NEG     // 负号->左结合
%right '^'    // 幂运算->右结合
%token ENDINP // 输入结束符'$'

// 语法结点的语义生成规则
// 靠后的生成式被优先使用
%%
// 起始符号
S   :// 空
    | input S
;

// 输入结束符
dollar  :ENDINP // '$'
;

input   : dollar
        | exp dollar {
            printf(">> %lf\n", $1); 
            printf("exp<< ");
        }
;

exp     :NUM                {$$ = $1;}
        | exp '+' exp        {$$ = $1 + $3;}
        | exp '-' exp        {$$ = $1 - $3;}
        | exp '*' exp        {$$ = $1 * $3;}
        | exp '/' exp        {$$ = $1 / $3;}
        | '-' exp  %prec NEG {$$ = -$2;}
        | exp '^' exp        {$$ = pow($1, $3);}
        | '(' exp ')'        {$$ = $2;}
;
%%

// 宏定义中代码的具体实现
int yylex(void)
{
    int token = getchar();
    //跳过间隔符号
    while(token == ' ' || token == '\t' || token == '\n') 
    {
        token = getchar();
    }
    //数字
    if(token == '.' || isdigit(token))
    {
        ungetc(token, stdin);
        scanf("%lf", &yylval);
        return NUM;
    }
    //$符号表示输入结束
    if(token == '$')
    {
        ungetc(token, stdin);
        scanf("%c", &yylval);
        return ENDINP;
    }
    //结束符
    if(token == EOF)
    {
        return 0;//若为0则退出
    }
    //其他
    return token;
}

void yyerror(char const *str)
{
    printf("%s\n", str);
}

int main(void)
{
    printf("exp<< ");
    return yyparse();
}