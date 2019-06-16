int fact(int n)
{
	if (n <= 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else
	{
		return n * f(n - 1);
	}
}

int main(int argc, char const *argv[])
{
	return fact(5);
}
