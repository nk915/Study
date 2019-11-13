#include <stdio.h>
#include <stdlib.h>

struct test
{
	int t;
	int tt;
};


int main()
{
	printf("sizeof(int)  %d\n", sizeof(int));
	printf("(int)sizeof(int)  %d\n", (int)sizeof(int));
	
	printf("sizeof(int*)  %d\n", sizeof(int*));
	printf("(int)sizeof(int*)  %d\n", (int)sizeof(int*));


	printf("sizeof(long)  %d\n", sizeof(long));
	


	char test[10]={0,};
	printf("chr[10]  %d\n", sizeof(test));
	printf("chr[10]  %d\n", (int)sizeof(test));
	

	char *test1[10]={0,};
	printf("chr*[10]  %d\n", sizeof(test1));
	printf("chr*[10]  %d\n", (int)sizeof(test1));
	


	struct test ttt;
	printf("test %d \n", sizeof(ttt));
	printf("(char*) test %d \n", sizeof((char *)&ttt));


	printf("long %d \n", sizeof(long));
	printf("long int %d \n", sizeof(long int));

	printf("\"test\" %d \n", sizeof("test"));
	return 0;
}

