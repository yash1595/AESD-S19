#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	printf("Command Line Program!\n");
	FILE* fp;
	char* filename = "comamnds.txt";
	fp = fopen(filename,"w");
	char ch='A';
	fprintf(fp,"%c",ch);
	fclose(fp);
	fp = fopen(filename,"a");
	char* input = (char*)malloc(20);
	if(input==NULL)fprintf(fp,"Error Allocating memory!\n");
	printf("Enter a string:");
	scanf("%[^\n]",input);
	fprintf(fp,"%s",input);
	fflush(fp);
	fclose(fp);
	fopen(filename,"r");
	if(ch=fgetc(fp))printf("%c\n",ch);
	fgets(input,20,fp);
	puts(input);
	fclose(fp);
	free(input);
	input=NULL;
	printf("Freed Memory\n");

	return 0;
}