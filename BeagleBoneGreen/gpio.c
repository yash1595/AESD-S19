
#include "main.h"

void gpio_on_off(uint8_t pin,uint8_t state)
{
	FILE *fp1;

	if(pin<8 && state<2)
	{
		fp1 = fopen(LED_Dir_Path[pin],"w");
		if(fp1==NULL) printf("Error in led\n");
		else
		printf("\n Success file open");
	}
	else
	{
		printf("\n Error in pin or state\n");
		return ;
	}

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",pin);
	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%s","out");
	fclose(fp1);

	fp1 = 	fopen(LED_Val_Path[pin],"w");
	if(fp1==NULL) printf("Error in led\n");
	else
	printf("\n Success file open 2");

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",state);

	fclose(fp1);

}
