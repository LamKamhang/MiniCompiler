/* declaration */
int g(int i, double b, char c);
int _int;
double _double;

char _char_array[1];
float *_float_ptr;
const int _const_int;
const int * _const_int_ptr;
int const * const _const_int_const_ptr;

/* definition */

int _d_int = 0;
double _d_double = 1.0;
double _e_double = 1e-2;
char *_d_char_ptr = "World";
char a = 'dsafs';
int;
/* function definition */
int main(int argc, char *argv[])
{
  if (argc == 0)
    {
      return 0;
    }
  else
    {
      if (argc == 1)
	{
	  return 1;
	}
      else if (argc == 2)
	{
	  return 2;
	}
      else
	{
	  if (argc >= 3)
	    if (argc == 3)
	      return 3;
	    else
	      {
		return g(argc, argc, argc);
	      }
	}
    }
  return 0;
}


int g(int i, double b, char c)
{
  switch (c)
    {
          case 'a':break;
          case 'b':i++;break;
          case 'c':b /= 2;break;
    default:{
      b = b*2 + (++i);
    }
    }
  for (; i > 3; --i)
    {
      while (1)
	{
	  break;
	}
	}
  return (int)(i+b+c);
}

