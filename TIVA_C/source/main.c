/* FreeRTOS 8.2 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 */

#include "main.h"
//uint32_t ui32Period;

//QueueHandle_t QUEUE;

//uint8_t I2CByte1,I2CByte2;
//
//struct Task
//{
//    TickType_t timestamp;
//    float temperature;
//    uint32_t led_count;
//    char name[30];
//    uint8_t tid;
//};
//
//struct Task global;
//

char c;

void my_uartHandler(void)
{
    TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);    //Enables individual timer interrupt

    uint32_t status = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, status);

            c = UARTCharGet(UART3_BASE);

            if(c=='a')
            {
                flag =1;
                UARTprintf("*******************ALERt[Temp]************************\n");
                function_check();
            }
            else if(c=='n')
            {
                UARTprintf("*******************NOALERT[TEMP]***********************\n");
                no_alert();
            }
            else if(c=='d')
            {
                flag1 = 1;
                UARTprintf("*******************ALERT[DISTANCE]************************\n");
                function_check();
            }

            else if(c=='o')
            {
               UARTprintf("*******************NOALERT[DISTANCE]***********************\n");
            }

            else if(c=='h')
            {
                UARTprintf("******************ALERT[SMOKE]****************************\n");
                servo();
            }
            else if(c=='t')
            {
                UARTprintf("******************NOALERT[SMOKE]****************************\n");
            }
            else if(c=='x')
            {
                UARTprintf("******************[SENSOR BROKEN]*****************************\n");
                RelayInit(PORTL,PIN3);
                RelayState(PORTL,PIN3,1);
                SysCtlDelay(1000000);
                RelayInit(PORTL,PIN3);
                RelayState(PORTL, PIN3,0);

            }
            else if(c=='y')
           {
               UARTprintf("**********[Communcation Broken TX of TIVA]************\n");
               RelayInit(PORTF,PIN3);
               RelayState(PORTF,PIN3,1);
               SysCtlDelay(1000000);
               RelayInit(PORTF,PIN3);
               RelayState(PORTF, PIN3,0);
            }

            else if(c=='z')
             {
                 UARTprintf("***************[Alert from Sensor]*************************\n");
                 RelayInit(PORTH,PIN2);
                 RelayState(PORTH,PIN2,1);
                 SysCtlDelay(1000000);
                 RelayInit(PORTH,PIN2);
                 RelayState(PORTH, PIN2,0);
             }

            else if(c=='r')
             {
                 UARTprintf("***************[Alert from Fingerprint]*************************\n");
                 servo_finger();

             }
            else if(c=='s')
             {
                 UARTprintf("***************[NoAlert Fingerprint]*************************\n");
                 servo();

             }


}


void Timer0IntHandler(void)
{
  //  UARTprintf("Entrance2 \n");
   // UARTprintf("\n%u",xTaskGetTickCount());
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterDisable();
    if(count_global<10)
     {
        UARTprintf("\nCommunication broken!!!\n");

     UARTCharPut(UART3_BASE, 'X');
     UARTCharPut(UART0_BASE, 'X');
     }    //IntDisable(INT_TIMER0A);
    IntMasterEnable();

}

void timer_init(){

 //   UARTprintf("Entrance\n");
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   //Enable clock to timer0 peripheral
    IntMasterEnable();
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //Enable timer in periodic mode
    TimerLoadSet(TIMER0_BASE, TIMER_A, 768000000);     //Load the timer with given count after which it will generate an interrupt,right now set as 1ms
    IntEnable(INT_TIMER0A);                             //Enable Timer0A interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);    //Enables individual timer interrupt
    TimerEnable(TIMER0_BASE, TIMER_A);                  //Starts timer

}

// Main function
//Black - PA4 , Red - PA5
int main(void)
    {
    flag_startup = 0;
    // Initialize system clock to 120 MHz

        output_clock_rate_hz = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),SYSTEM_CLOCK);
        ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
             SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
             SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

             GPIOPinConfigure(GPIO_PA0_U0RX);
             GPIOPinConfigure(GPIO_PA1_U0TX);
             GPIOPinConfigure(GPIO_PA4_U3RX);
             GPIOPinConfigure(GPIO_PA5_U3TX);

             GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 );
             UARTConfigSetExpClk(UART0_BASE, output_clock_rate_hz, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
             UARTConfigSetExpClk(UART3_BASE, output_clock_rate_hz, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));


             //Configure Trigger pin
                SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
                SysCtlDelay(3);
                GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_3);

                //Configure Echo pin
                SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
                SysCtlDelay(3);
                GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_2);
                GPIOIntEnable(GPIO_PORTN_BASE, GPIO_PIN_2);
                GPIOIntTypeSet(GPIO_PORTN_BASE, GPIO_PIN_2,GPIO_BOTH_EDGES);
                GPIOIntRegister(GPIO_PORTN_BASE,inputInt);

        // Initialize the GPIO pins for the Launchpad
        PinoutSet(0,0);
        UARTStdioConfig(0, 115200, output_clock_rate_hz);

      //  while(1);

        timer_init();
        i2c_init(); //I2C initialisation by portb and temp i2c by address0x48
        startup();

        SysCtlDelay(1000000);
        if(flag_startup == 0 && smoke_test_flag==0)
        {

            //servo();
         UARTprintf("Startup Success\n");
         xTaskCreate(logger, (const portCHAR *)"Logs",1200, NULL, 1, NULL); //logger task initiated
         xTaskCreate(temp_task, (const portCHAR *)"Temp",1200, NULL, 1, NULL); //temp task initiated
         xTaskCreate(ultrasonic_task, (const portCHAR *)"Ultra",1200, NULL, 1, NULL); //temp task initiated
         xTaskCreate(smoke_task, (const portCHAR *)"smoke",1200, NULL, 1, NULL); //temp task initiated
         xTaskCreate(fingerprint_task, (const portCHAR *)"Finger",1200, NULL, 1, NULL); //temp task initiated
//       // servo();
        vTaskStartScheduler();
        }
         else
         {
             UARTprintf("Startup Failed\n");
         }
        return 0;
}


/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}




