int fib(int n)
{
	if (n - 2) //if n<=2
	{
		if (n - 1)
		{
			return fib(n - 1) + fib(n - 2);
		}
	}
	else //n<=2
	{
		return 1;
	}
}
int main()
{
	int n = 5;
	int ret = 0;
	ret = fib(n);

	return ret;
}