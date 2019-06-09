int a(int b, int c){return 0;}
int main()
{
	int flag = 0;
	if(flag)
	{
		a(2, 3);
	}
	else
	{
		a(1, 2);
	}
	
	return a(1, 2);
}