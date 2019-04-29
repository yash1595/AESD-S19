#include "main.h"

#define E  0x20
#define RS 0x10
#define LCDDATA (*((volatile uint32_t *)0x400613FC))
#define LCDCMD (*((volatile uint32_t *)0x4005C0C0))

void OutCmd(unsigned char );
void LCD_OutChar(unsigned char );
void LCD_Clear(void);
//void LCD_OutString(char );
void LCD_OutUDec(uint32_t );
void PortFunctionInit(void);

void OutCmd(unsigned char command){
  LCDDATA = command;
  LCDCMD = 0;
  SysCtlDelay(240);
  LCDCMD = E;
  SysCtlDelay(240);
  LCDCMD = 0;
  SysCtlDelay(1600);
}

void LCD_OutChar(unsigned char letter){
  LCDDATA = letter;
  LCDCMD = RS;
  SysCtlDelay(240);
  LCDCMD = E+RS;
  SysCtlDelay(240);
  LCDCMD = RS;
  SysCtlDelay(1600);
}

void LCD_Clear(void){
  OutCmd(0x01);
  SysCtlDelay(64000);
  OutCmd(0x02);
  SysCtlDelay(64000);
}

void LCD_OutString(char *pt){
  while(*pt){
    LCD_OutChar(*pt);
    pt++;
  }
}

void LCD_OutUDec(uint32_t n){

  if(n >= 10){
    LCD_OutUDec(n/10);
    n = n%10;
  }
  LCD_OutChar(n+'0');
}
//volatile uint32_t ui32SysClkFreq;


void PortFunctionInit(void){
    volatile uint32_t ui32Loop;
    //ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9|SYSCTL_RCGCGPIO_R4|SYSCTL_RCGCGPIO_R12;

    ui32Loop = SYSCTL_RCGCGPIO_R;
    GPIO_PORTN_DIR_R |= 0x01;
    GPIO_PORTN_DEN_R |= 0x01;
    GPIO_PORTK_AMSEL_R &= ~0xFF;
    GPIO_PORTE_AHB_AMSEL_R &= ~0x30;
    GPIO_PORTK_PCTL_R = 0x00000000;
    GPIO_PORTE_AHB_PCTL_R &= ~0xFF000000;
    GPIO_PORTK_DIR_R = 0xFF;
    GPIO_PORTE_AHB_DIR_R |= 0x30;
    GPIO_PORTK_AFSEL_R = 0x00;
    GPIO_PORTE_AHB_AFSEL_R &= ~0x30;
    GPIO_PORTK_DEN_R = 0xFF;
    GPIO_PORTE_AHB_DEN_R |= 0x30;
    GPIO_PORTK_DR8R_R = 0xFF;
    GPIO_PORTE_AHB_DR8R_R |= 0x30;

    LCDCMD = 0;
    SysCtlDelay(600000);
    OutCmd(0x30);
    SysCtlDelay(200000);
    OutCmd(0x30);
    SysCtlDelay(6400);
    OutCmd(0x30);
    SysCtlDelay(6400);
    OutCmd(0x38);
    OutCmd(0x10);
    OutCmd(0x0C);
    OutCmd(0x06);
}

//Function for if alert occurs due to temperature
void temp_func()
{
    UARTprintf("LCD Print\n");
    PortFunctionInit();
//  GPIO_PORTN_DATA_R |= 0x01;
    SysCtlDelay(1500000);
    LCD_Clear();
    //while(flag==1)
    {
        GPIO_PORTN_DATA_R ^= 0x01;
        LCD_OutString("Alert Temp");
        SysCtlDelay(15000000);
        LCD_Clear();


    }
}

//Function if no alert occurs due to temperature
void no_alert()
{
    PortFunctionInit();
    SysCtlDelay(200000);
    LCD_Clear();
    GPIO_PORTN_DATA_R ^= 0x01;
    LCD_OutString("No Alert Temp");
    SysCtlDelay(15000000);
    LCD_Clear();

}
