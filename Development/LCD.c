//#define PART_TM4C123GH6PM

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"

#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"


//#include "utils/uartstdio.c"
#include <string.h>


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/inc/tm4c1294ncpdt.h"
#include "driverlib/sysctl.h"

/* ============================ Defined constants ============================ */
#define E  0x20                                             /* Enable on PE5 */
#define RS 0x10                                             /* Read Select (RS) on PE4 */
#define LCDDATA (*((volatile uint32_t *)0x400613FC))                            /* Parallel Data on PORTK (PK0 - PK7)  */
#define LCDCMD (*((volatile uint32_t *)0x4005C0C0))                         /* Commands on PE5 and PE4 */

/* ============================ Function prototypes ============================ */
void OutCmd(unsigned char );
void LCD_OutChar(unsigned char );
void LCD_Clear(void);
//void LCD_OutString(char );
void LCD_OutUDec(uint32_t );
void PortFunctionInit(void);

/* ============================ LCD commands function ============================ */
void OutCmd(unsigned char command){
  LCDDATA = command;
  LCDCMD = 0;                                                   /* E=0, R/W=0, RS=0 */
  SysCtlDelay(240);                                         /* SysTick_Wait(T6us); wait 6us */
  LCDCMD = E;                                               /* E=1, R/W=0, RS=0 */
  SysCtlDelay(240);                                         /* SysTick_Wait(T6us); wait 6us */
  LCDCMD = 0;                                               /* E=0, R/W=0, RS=0 */
  SysCtlDelay(1600);                                            /* SysTick_Wait(T40us); wait 40us */
}

/* ============================ LCD Character function ============================ */
/* Output a character to the LCD, Inputs: letter is ASCII character, 0 to 0x7F, Outputs: none */
void LCD_OutChar(unsigned char letter){
  LCDDATA = letter;
  LCDCMD = RS;                                              /* E=0, R/W=0, RS=1 */
  SysCtlDelay(240);                                         /* SysTick_Wait(T6us); wait 6us */
  LCDCMD = E+RS;                                            /* E=1, R/W=0, RS=1 */
  SysCtlDelay(240);                                         /* SysTick_Wait(T6us); wait 6us */
  LCDCMD = RS;                                              /* E=0, R/W=0, RS=1 */
  SysCtlDelay(1600);                                            /* SysTick_Wait(T40us); wait 40us */
}

/*============================ Clear the LCD, Inputs: none, Outputs: none ============================ */
void LCD_Clear(void){
  OutCmd(0x01);                                             /* Clear Display */
  SysCtlDelay(64000);                                           /* SysTick_Wait(T1600us); wait 1.6ms */
  OutCmd(0x02);                                             /* Cursor to home */
  SysCtlDelay(64000);                                           /* SysTick_Wait(T1600us); wait 1.6ms */
}

/* ============================ LCD_OutString ============================  */
/* Output String (NULL termination), Input: pointer to a NULL-terminated string to be transferred, Output: none */
void LCD_OutString(char *pt){
  while(*pt){
    LCD_OutChar(*pt);
    pt++;
  }
}

/* ============================ LCD_OutUDec ============================ */
/* Output a 32-bit number in unsigned decimal format, Input: 32-bit number to be transferred, Output: none
   Variable format 1-10 digits with no space before or after */
void LCD_OutUDec(uint32_t n){
/*This function uses recursion to convert decimal number of unspecified length as an ASCII string */
  if(n >= 10){
    LCD_OutUDec(n/10);
    n = n%10;
  }
  LCD_OutChar(n+'0');                                           /* n is between 0 and 9 */
}

volatile uint32_t ui32SysClkFreq;

void PortFunctionInit(void){
    volatile uint32_t ui32Loop;
    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
                                                    /* 120MHz */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9|SYSCTL_RCGCGPIO_R4|SYSCTL_RCGCGPIO_R12;
                                                    /* Do a dummy read to insert a few cycles after
                                                    enabling the peripheral */
/* ============================ GPIO Port configuration ============================ */
    ui32Loop = SYSCTL_RCGCGPIO_R;
    GPIO_PORTN_DIR_R |= 0x01;                                   /* PN0 direction as output */
    GPIO_PORTN_DEN_R |= 0x01;                                   /* PN0 data enable */
    GPIO_PORTK_AMSEL_R &= ~0xFF;                                    /* disable analog function on PK7-0 */
    GPIO_PORTE_AHB_AMSEL_R &= ~0x30;                                /* disable analog function on PE5-4 */
    GPIO_PORTK_PCTL_R = 0x00000000;                                 /* configure PK7-0 as GPIO */
    GPIO_PORTE_AHB_PCTL_R &= ~0xFF000000;                               /* configure PA7-6 as GPIO */
    GPIO_PORTK_DIR_R = 0xFF;                                    /* set direction register */
    GPIO_PORTE_AHB_DIR_R |= 0x30;
    GPIO_PORTK_AFSEL_R = 0x00;                                  /* regular port function */
    GPIO_PORTE_AHB_AFSEL_R &= ~0x30;
    GPIO_PORTK_DEN_R = 0xFF;                                    /* enable digital port */
    GPIO_PORTE_AHB_DEN_R |= 0x30;
    GPIO_PORTK_DR8R_R = 0xFF;                                   /* enable 8 mA drive */
    GPIO_PORTE_AHB_DR8R_R |= 0x30;

/* ============================ LCD controller initilization ============================ */
    LCDCMD = 0;                                         /* E=0, R/W=0, RS=0 */
    SysCtlDelay(600000);                                        /* Wait >15 ms after power is applied */
    OutCmd(0x30);                                           /* command 0x30 = Wake up */
    SysCtlDelay(200000);                                        /*must wait 5ms, busy flag not available */
    OutCmd(0x30);                                           /* command 0x30 = Wake up #2 */
    SysCtlDelay(6400);                                      /* must wait 160us, busy flag not available */
    OutCmd(0x30);                                           /* command 0x30 = Wake up #3 */
    SysCtlDelay(6400);                                      /* must wait 160us, busy flag not available */
    OutCmd(0x38);                                           /* Function set: 8-bit/2-line */
    OutCmd(0x10);                                           /* Set cursor */
    OutCmd(0x0C);                                           /* Display ON; Cursor ON */
    OutCmd(0x06);                                           /* Entry mode set */
}

/* ============================ Main function ============================ */
int main(void){
    PortFunctionInit();                                     /* Initialize ports */
    GPIO_PORTN_DATA_R |= 0x01;                                  /*  LED ON */
    SysCtlDelay(200000);                                        /* Give some delay */
    LCD_Clear();                                            /* Clear the LCD screen */
    LCD_OutString("STRING ONE");                                    /* Write a string on the screen */
    SysCtlDelay(80000000);                                      /* keep the string for some time on the screen */
    while(1)                                            /* Infinite while loop */
    {
        GPIO_PORTN_DATA_R ^= 0x01;                              /* keep blinking the LED at PN1 */
        SysCtlDelay(40000000);                                  /* give some delay for blink */
        LCD_Clear();                                        /* clear the LCD */
        LCD_OutString("STRING TWO");                                /* show a string on LCD */
        SysCtlDelay(80000000);                                  /* delay to keep the string on LCD */
    }
}
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
