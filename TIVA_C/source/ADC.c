/*
 * ADC.c
 *
 *  Created on: Apr 19, 2019
 *      Author: yashm
 */


/************
 * Add to main.h:
 * /************ ADC ******************

 *
 *
 *  also add to main.h
 */
#include "main.h"

void SetUpADC(void)
{
//    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
//                                       SYSCTL_OSC_MAIN |
//                                       SYSCTL_USE_PLL |
//                                       SYSCTL_CFG_VCO_480), 20000000);
    // The ADC0 peripheral must be enabled for use.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

        //
        // For this example ADC0 is used with AIN0 on port E7.
        // The actual port and pins used may be different on your part, consult
        // the data sheet for more information.  GPIO port E needs to be enabled
        // so these pins can be used.
        // TODO: change this to whichever GPIO port you are using.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

        //
        // Select the analog ADC function for these pins.
        // Consult the data sheet to see which functions are allocated per pin.
        // TODO: change this to select the port/pin you are using.
        //
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

        //
        // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
        // will do a single sample when the processor sends a signal to start the
        // conversion.  Each ADC module has 4 programmable sequences, sequence 0
        // to sequence 3.  This example is arbitrarily using sequence 3.
        //
        ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

        //
        // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
        // single-ended mode (default) and configure the interrupt flag
        // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
        // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
        // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
        // sequence 0 has 8 programmable steps.  Since we are only doing a single
        // conversion using sequence 3 we will only configure step 0.  For more
        // information on the ADC sequences and steps, reference the datasheet.
        //
        ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                                 ADC_CTL_END);

        //
        // Since sample sequence 3 is now configured, it must be enabled.
        //
        ADCSequenceEnable(ADC0_BASE, 3);

        //
        // Clear the interrupt status flag.  This is done to make sure the
        // interrupt flag is cleared before we sample.
        //
        ADCIntClear(ADC0_BASE, 3);
}

uint32_t ADCConvert(void)
{

        //
        // Trigger the ADC conversion.
        //
        ADCProcessorTrigger(ADC0_BASE, 3);

        //
        // Wait for conversion to be completed.
        //
        while(!ADCIntStatus(ADC0_BASE, 3, false));

        //
        // Clear the ADC interrupt flag.
        //
        ADCIntClear(ADC0_BASE, 3);

        //
        // Read ADC Value.
        //
        ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);

        //
        // Display the AIN0 (PE3) digital value on the console.
        //
        //UARTprintf("AIN0 = %4d\n", pui32ADC0Value[0]);
        return pui32ADC0Value[0];

        //SysCtlDelay(30000000);  //Approx 1s delay

}

uint8_t SmokeStartUpTest(void)
{
    UARTprintf("SmokeStartUpTEST\n");
    SetUpADC();
    static uint32_t counts=0;
    while(counts<10000)
        {
            ADCProcessorTrigger(ADC0_BASE, 3);
            while(!ADCIntStatus(ADC0_BASE, 3, false));
            ADCIntClear(ADC0_BASE, 3);
            ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
            counts+=1;
            if(pui32ADC0Value[0]<900)
            {
                UARTprintf("Start-up Test failed at value:%d\n",pui32ADC0Value[0]);
                return 1;
            }
        }
    UARTprintf("Start-up Test passed\n");
    return 0;
}

void smoke_task(void *pvParameters)
{
    char Text_Print[150];
    char str[10];
    int count=0;
    uint32_t smoke_val=0;
    UARTprintf("\nSmoke Task Started\n");

    static smoke_struct smoke_info;

    while(1)
        {

        smoke_val=ADCConvert();
            if(smoke_val>900)
            {
                strcpy(str,"S");

                snprintf(Text_Print, sizeof(Text_Print), "%d",(int)(smoke_val/100));
                strcat(str,Text_Print);
                strcat(str,"S");
                strcpy(smoke_info.my_name,str);
                bzero(str,sizeof(str));
//                if(smoke_val > 2200)
//                {
//
//                    servo_alarm();
//
//                }
//                IntMasterEnable();
//                IntEnable(INT_UART3);
//                UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
                if(xQueueSend(log_q,(void *)&smoke_info, 100) != pdTRUE)
                {
                    UARTprintf("\nLog Error");
                }




            }
            else
            {
                UARTprintf("Smoke sensor Failed\n");
                UARTCharPut(UART3_BASE,f);
            }
            vTaskDelay(987);
        }

}
