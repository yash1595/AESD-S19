/*
 * LogQueue.c
 *
 *  Created on: Apr 10, 2019
 *      Author: mentor
 */

#include "main.h"

volatile char* convert_structure;
volatile char* convert_structure1;
volatile char* convert_structure2;
void logger(void *pvParameters)
{
    //int length;
    //static char Text_Print[200];
    log_q = xQueueCreate(10, sizeof(ultrasonic));
    log_q1 = xQueueCreate(10, sizeof(structure_sent));
    log_q2 = xQueueCreate(10, sizeof(finger));
    static structure_sent log_info;
    static ultrasonic log_info1;
    static finger log_info2;
    if(log_q == 0)
    {
     UARTprintf("\nLOG Queue Creation Failed");
    }

    else
    {
     SysCtlDelay(1000000);
     UARTprintf("\nLOG Queue Created\n");
    }
    convert_structure = (uint32_t *)&log_info;
    convert_structure1 = (uint32_t *)&log_info1;
    convert_structure2 = (uint32_t *)&log_info2;
    while(1)
    {

        if(xQueueReceive(log_q, (void *)&log_info1, 0) == pdTRUE)
        {

          UART_Communication1(convert_structure1,sizeof(ultrasonic));
        }
        else if(xQueueReceive(log_q, (void *)&log_info, 0) == pdTRUE)
        {

          UART_Communication(convert_structure,sizeof(structure_sent));
        }
        else if(xQueueReceive(log_q, (void *)&log_info2, 0) == pdTRUE)
        {
          UART_Communication(convert_structure2,sizeof(finger));
        }

    }

}

void UART_Communication(char *convert_structure, int length)
{

    //UARTprintf("Length is %d\n",length);
    while(length != 0)
    {
      UARTCharPut(UART0_BASE, *convert_structure);
      UARTCharPut(UART3_BASE, *convert_structure);
      convert_structure++;
      length--;
    }
}


void UART_Communication1(char *convert_structure1, int length)
{
   // UARTprintf("Length is %d\n",length);
    while(length != 0)
    {
      UARTCharPut(UART0_BASE, *convert_structure1);
      UARTCharPut(UART3_BASE, *convert_structure1);
      convert_structure1++;
      length--;

    }
}


void UART_Communication2(char *convert_structure2, int length)
{
   // UARTprintf("Length is %d\n",length);
    while(length != 0)
    {
      UARTCharPut(UART0_BASE, *convert_structure2);
      UARTCharPut(UART3_BASE, *convert_structure2);
      convert_structure2++;
      length--;

    }
}



void function_check()
{
    if(flag == 1)
    {

     temp_func();
     flag = 0;
    }
    else if(flag1 == 1)
    {
      UARTprintf("Alert from Distance\n");
      RelayInit(PORTF,PIN2);
      RelayState(PORTF,PIN2,1);
      SysCtlDelay(1000000);
      flag1 = 0;
      RelayInit(PORTF,PIN2);
      RelayState(PORTF, PIN2,0);
    }

}


void RelayInit(uint32_t PORT, uint32_t PIN)
{
    GPIOPinTypeGPIOOutput(PORT, PIN);
}

void RelayState(uint32_t PORT,uint32_t PIN,uint8_t state)
{
    if(state==1)
        GPIOPinWrite(PORT,PIN,PIN);
    else
        GPIOPinWrite(PORT,PIN,0);
}

void servo()
{
       SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
       GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
       GPIOPinConfigure(GPIO_PG1_M0PWM5);
        PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN |PWM_GEN_MODE_NO_SYNC);
        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 100000);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) /6);
        PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
        PWMGenEnable(PWM0_BASE, PWM_GEN_2);
      //  while(1)
        {
         // UARTprintf("Normal\n");
         // SysCtlDelay(1000000);
          PWMOutputInvert(PWM0_BASE, PWM_OUT_5_BIT, true);
       //   SysCtlDelay((output_clock_rate_hz * 5) / 3);
       //   SysCtlDelay(1000000);

         // PWMOutputInvert(PWM0_BASE, PWM_OUT_5_BIT, false);
          SysCtlDelay(1000000);
              // UARTprintf("Normal 2\n");
         // SysCtlDelay((output_clock_rate_hz * 5) / 3);
        }


}


void servo_finger()
{
       SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
       GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
       GPIOPinConfigure(GPIO_PG1_M0PWM5);
        PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN |PWM_GEN_MODE_NO_SYNC);
        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 100000);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5,PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2) /6);
        PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
        PWMGenEnable(PWM0_BASE, PWM_GEN_2);
      //  while(1)
        {
         // UARTprintf("Normal\n");
         // SysCtlDelay(1000000);
          PWMOutputInvert(PWM0_BASE, PWM_OUT_5_BIT, false);
       //   SysCtlDelay((output_clock_rate_hz * 5) / 3);
       //   SysCtlDelay(1000000);

         // PWMOutputInvert(PWM0_BASE, PWM_OUT_5_BIT, false);
          SysCtlDelay(1000000);
              // UARTprintf("Normal 2\n");
         // SysCtlDelay((output_clock_rate_hz * 5) / 3);
        }


}

void servo_alarm(void)
{

        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinConfigure(GPIO_PF1_M0PWM1);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
        PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);

        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 64000);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,
                         PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) / 4);
        PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
        PWMGenEnable(PWM0_BASE, PWM_GEN_0);


}


void servo_alarm_off(void)
{


//    PWMGenDisable(PWM0_BASE, PWM_GEN_0);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);


}
