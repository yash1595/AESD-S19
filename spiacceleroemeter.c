#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
//Includes
#include <stdint.h>
#include <stdbool.h>

//#include "drivers/pinout.h"
#include "utils/uartstdio.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"

// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

//*******************************************************
//                      ADXL345 Definitions
//*******************************************************

#define READ    0x80

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// Configure SSI0 in master Freescale (SPI) mode.  This example will send out
// 3 bytes of data, then wait for 3 bytes of data to come in.  This will all be
// done using the polling method.
//
//*****************************************************************************
int
main(void)
{
    uint32_t x_value_raw=0;
    uint32_t y_value_raw=0;
    uint32_t z_value_raw=0;
    double x_value;
    uint32_t dataready;
    uint32_t pui32DataRx=0;

    uint32_t ui32SysClock;
        ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                               SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480
                                               ), 25000000);
    InitConsole();
    UARTprintf("SSI ->\n");
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
    GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);
    SSIConfigSetExpClk(SSI0_BASE,ui32SysClock , SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI0_BASE);

    //wr
      GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
      SSIDataPut(SSI0_BASE,0x20);
      while(SSIBusy(SSI0_BASE));
      SSIDataGet(SSI0_BASE, &pui32DataRx);
      SSIDataPut(SSI0_BASE,0x6D);
      while(SSIBusy(SSI0_BASE));
      SSIDataGet(SSI0_BASE, &pui32DataRx);
      GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3,GPIO_PIN_3 );

    //read
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
    SSIDataPut(SSI0_BASE, READ|0x20);
    while(SSIBusy(SSI0_BASE));
    SSIDataGet(SSI0_BASE, &pui32DataRx);
    SSIDataPut(SSI0_BASE,0xFF);
    while(SSIBusy(SSI0_BASE));
    SSIDataGet(SSI0_BASE, &pui32DataRx);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3,GPIO_PIN_3 );



    UARTprintf("%x\n",pui32DataRx);
    while(1){
//        x_value_raw = pui32DataRx | (pui32DataRx>>8);
//      x_value = x_value_raw*0.00390625; //full resolution mode scaling
//
//
////        SSIDataPut(SSI0_BASE, READ|DATAY0);
////        SSIDataGet(SSI0_BASE, &pui32DataRx);
////        while(SSIBusy(SSI0_BASE)){}
////        SSIDataPut(SSI0_BASE, READ|DATAY1);
////        SSIDataGet(SSI0_BASE, &pui32DataRx);
////        while(SSIBusy(SSI0_BASE)){}
////        y_value_raw = pui32DataRx[3] | (pui32DataRx[2]>>8);
//////      y_value = y_value_raw*0.00390625; //full resolution mode scaling
////
////        SSIDataPut(SSI0_BASE, READ|DATAZ0);
////        SSIDataGet(SSI0_BASE, &pui32DataRx[4]);
////        while(SSIBusy(SSI0_BASE)){}
////        SSIDataPut(SSI0_BASE, READ|DATAZ1);
////        SSIDataGet(SSI0_BASE, &pui32DataRx[5]);
////        while(SSIBusy(SSI0_BASE)){}
////        z_value_raw = pui32DataRx[5] | (pui32DataRx[4]>>8);
//////      z_value = z_value_raw*0.00390625; //full resolution mode scaling
//
//        UARTprintf("%x\t%d\t%d\r",x_value_raw,y_value_raw,z_value_raw);
//
//            SysCtlDelay(100);
    }

    //return(0);
}

void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
