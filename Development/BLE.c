/*
 * BLE.c
 *
 *  Created on: Apr 18, 2019
 *      Author: yashm
 */

#include "BLE.h"

#define ALERT       "1"

#define OK_WAIT     "OK+WAIT"
#define CHECK_CMD   "AT"
#define CHECK_EVT   "OK"
#define CHECK_ERR   "Not Initialized"
#define CHECK_MSG   "Initialized"

#define ADDR_CMD     "AT+ADDR?"
#define NAME_CMD     "AT+NAME?"
#define NAME_CHN     "AT+NAME=HC-08"
#define MODE_ID      "AT+MODE0"
//#define ADTY_CMD     "AT+ADTY"


#define ADDR_PRE     "AT+ADDR=1234567890AB"
#define ADDR_DISC    "AT+ADDR=?"
#define ADDR_MSG     "OKsetADDR"
//#define LIST_ERR     "Interval Not Set"
//#define LIST_MSG     "Interval set"

#define BAUD_CMD     "AT+BAUD?"
#define BAUD_SET     "AT+BAUD0"
#define BAUD_EVT     "OK+Get:0"
#define BAUD_ERR     "Baud Not Set"
#define BAUD_MSG     "Baud set"

#define ADTY_CMD     "AT+ADTY?"
#define ADTY_MSG     "OK+Set:0"
#define ADVT_CMD     "AT+ADC[3]?"
#define ADVT_EVT     "OK+Set:0"
#define ADVT_ERR     "Adv Int err"
#define ADVT_MSG     "Adv Int Set"



#define AT_ROLE1    "AT+ROLE1"
#define AT_IMME1    "AT+IMME1"

void BLE_UART_Init(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);    //PC-6 RX
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);    //PC-7 TX

    GPIOPinConfigure(GPIO_PC6_U5RX);
    GPIOPinConfigure(GPIO_PC7_U5TX);
    MAP_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    MAP_UARTConfigSetExpClk(UART5_BASE, SYSTEM_CLOCK, 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
}

void BLESendAlert(void)
{
    UARTCharPut(UART5_BASE,"A");
}

void SendATCommands(void)
{
    static uint32_t i=0,j=0;
    char a[100];

    for(i=0;i<strlen(CHECK_CMD);i+=1)
    {
        UARTCharPut(UART5_BASE,CHECK_CMD[i]);
    }

    for(i=0;i<strlen(CHECK_EVT)+2;i+=1)
    {
        a[i]=UARTCharGet(UART5_BASE);
    }

    for(i=0;i<strlen(CHECK_EVT)+2;i+=1)
    {
        UARTprintf("%c",a[i]);
    }

    //Address
    for(i=0;i<strlen(ADDR_CMD);i+=1)
    {
        UARTCharPut(UART5_BASE,ADDR_CMD[i]);
    }

    for(i=0;i<strlen(ADDR_PRE)+2;i+=1)
    {
        a[i]=UARTCharGet(UART5_BASE);
    }

    for(i=0;i<strlen(ADDR_PRE)+2;i+=1)
    {
        UARTprintf("%c",a[i]);
    }

    //Name Check
    for(i=0;i<strlen(NAME_CMD);i+=1)
    {
        UARTCharPut(UART5_BASE,NAME_CMD[i]);
    }

    for(i=0;i<strlen(NAME_CHN)+2;i+=1)
    {
        a[i]=UARTCharGet(UART5_BASE);
    }

    for(i=0;i<strlen(NAME_CHN)+2;i+=1)
    {
        UARTprintf("%c",a[i]);
    }


    //BAUD check
    for(i=0;i<strlen(BAUD_CMD);i+=1)
    {
        UARTCharPut(UART5_BASE,BAUD_CMD[i]);
    }

    for(i=0;i<strlen(BAUD_EVT)+2;i+=1)
    {
        a[i]=UARTCharGet(UART5_BASE);
    }

    for(i=0;i<strlen(BAUD_EVT)+2;i+=1)
    {
        UARTprintf("%c",a[i]);
    }


    //Mode
    for(i=0;i<strlen(MODE_ID);i+=1)
    {
        UARTCharPut(UART5_BASE,MODE_ID[i]);
    }

    for(i=0;i<strlen(ADVT_EVT)+2;i+=1)
    {
        a[i]=UARTCharGet(UART5_BASE);
    }

    for(i=0;i<strlen(ADVT_EVT)+2;i+=1)
    {
        UARTprintf("%c",a[i]);
    }

}
