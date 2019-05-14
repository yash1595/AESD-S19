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
void func0()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	//gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func1()
{
	gpio_on_off(gpio49,1);
	// gpio_on_off(gpio66,1);
	// gpio_on_off(gpio67,1);
	// gpio_on_off(gpio68,1);
	// gpio_on_off(gpio69,1);
	// gpio_on_off(gpio44,1);
	// gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func2()
{
	//gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	//gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func3()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
//	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
//	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func4()
{
	gpio_on_off(gpio49,1);
	//gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
//	gpio_on_off(gpio44,1);
	//gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func5()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
//	gpio_on_off(gpio27,1);
//	gpio_on_off(gpio117,1);
}
void func6()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	//gpio_on_off(gpio117,1);
}
void func7()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	//gpio_on_off(gpio67,1);
//	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
//	gpio_on_off(gpio44,1);
//	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func8()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func9()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	//gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void alloff()
{
	gpio_on_off(gpio49,0);
	gpio_on_off(gpio66,0);
	gpio_on_off(gpio67,0);
	gpio_on_off(gpio68,0);
	gpio_on_off(gpio69,0);
	gpio_on_off(gpio44,0);
	gpio_on_off(gpio27,0);
	gpio_on_off(gpio117,0);
}

int main()
{
	alloff();
  while(1)
  {

		func9();


  }
}
