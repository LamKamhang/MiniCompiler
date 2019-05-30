#include "parser.hpp"
#include <stdio.h>

extern FILE* yyin;
extern FILE* yyout;

int main (int argc, char **argv)
{
	if (argc >= 2)
	{
		if ((yyin = fopen(argv[1], "r")) == NULL)
		{
			fprintf(stderr, "Cannot open file %s\n", argv[1]);
			return 1;
		}
		if (argc >= 3)
		{
			yyout = fopen(argv[2], "w");
		}
	}

	yyparse();

	if (argc >= 2)
	{
		fclose(yyin);
		if (argc >= 3) 
			fclose(yyout);
	}

	return 0;
}
