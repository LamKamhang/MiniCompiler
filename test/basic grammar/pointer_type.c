int main(int argc, char const *argv[])
{
	int a = 10;
	int *pa = &a;
	*pa = 5;
	return a;
}
