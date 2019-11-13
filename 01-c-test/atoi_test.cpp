#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *name = "Vikram";
	char buf[1024] = "1\n";

	int i=-1;	
	int b=-1;
	i = atoi(name);
	b = atoi(buf);
	printf("%d \n",i);
	printf("%d \n",b);




	return 0;
}

