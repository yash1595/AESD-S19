
#include "main.h"

//#include "includes.h"
const double temp = 1.0/80.0;
volatile uint32_t pulse;
volatile uint8_t echowait=0;
static ultrasonic ultra_info;

void Captureinit()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
  SysCtlDelay(3);
  TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);
  TimerEnable(TIMER2_BASE,TIMER_A);
}

void ultrasonic_task(void *pvParameters)
{
  char Text_Print[150];
  char str1[10];
  Captureinit();
  UARTprintf("\Ultrasonic Started\n");
  int count = 0;
  while(1)
  {
     // count = xTaskGetTickCount();
     // count = count -1;
//      UARTprintf("%d\n",count);
//      if(count%10000==0)
//        {
//            strcpy(str1,"A");
//        }
//      else
      {
          strcpy(str1,"D");
      }


      if(echowait != 1)
       {
       GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_PIN_3);
       SysCtlDelay(266);
       GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
       while(echowait != 0);
       pulse =(uint32_t)(temp * pulse);
       pulse = pulse / 58;
       if(pulse > 0)
       {
      //ultrasonic_value();
      snprintf(Text_Print, sizeof(Text_Print), "%d",pulse);
      strcat(str1,Text_Print);
      strcat(str1,"D");
      strcpy(ultra_info.ultra_name,str1);
      bzero(str1,sizeof(str1));
     // UARTprintf("\nTempTask Started and distance\n");
      //IntMasterEnable();
      //IntEnable(INT_UART3);
      //UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
      if(xQueueSend(log_q,(void *)&ultra_info, 100) != pdTRUE)
         {
             UARTprintf("\nLog Error");
         }

       }
       else
       {

           UARTprintf("Ultrasonic Failed\n");
           UARTCharPut(UART3_BASE,u);
       }
    }
      vTaskDelay(547);

  }
}

uint32_t ultrasonic_task1()
{
  char Text_Print[150];
  Captureinit();

  while(1)
  {
      char str1[150] = {'D'};
      if(echowait != 1)
       {
       GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_PIN_3);
       SysCtlDelay(266);
       GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
       while(echowait != 0);
       pulse =(uint32_t)(temp * pulse);
       pulse = pulse / 58;

       return pulse;
       }
    vTaskDelay(1000);
  }

}

void inputInt(){
  //Clear interrupt flag. Since we only enabled on this is enough
  GPIOIntClear(GPIO_PORTN_BASE, GPIO_PIN_2);

  /*
    If it's a rising edge then set he timer to 0
    It's in periodic mode so it was in some random value
  */
  if ( GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2) == GPIO_PIN_2){
    HWREG(TIMER2_BASE + TIMER_O_TAV) = 0; //Loads value 0 into the timer.
    TimerEnable(TIMER2_BASE,TIMER_A);
    echowait=1;
  }
  /*
    If it's a falling edge that was detected, then get the value of the counter
  */
  else{
    pulse = TimerValueGet(TIMER2_BASE,TIMER_A); //record value
    TimerDisable(TIMER2_BASE,TIMER_A);
    echowait=0;
  }
}



uint32_t CheckTest(void)
{
  char Text_Print[150];
  Captureinit();
  uint32_t counts=0,sum=0;
  uint32_t pulse_array[10];
  while(counts<10000)
  {
      char str1[150] = {'D'};
      if(echowait != 1)
       {
           GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_PIN_3);
           SysCtlDelay(266);
           GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
           while(echowait != 0 );
           pulse =(uint32_t)(temp * pulse);
           pulse = pulse / 58;
           sum+=pulse;
          // UARTprintf("Sum:%d\n",sum);
       }
      counts+=1;
      if(sum>0)
          return 0;
   }
 return 1;
}



//uint32_t CheckTest(void)
//{
//  char Text_Print[150];
//  Captureinit();
//  uint32_t counts=0,sum=0;
//  uint32_t pulse_array[10];
//  while(counts<10000)
//  {
//      char str1[150] = {'D'};
//      if(echowait != 1)
//       {
//           GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_PIN_3);
//           SysCtlDelay(266);
//           GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
//           while(echowait != 0 );
//           pulse =(uint32_t)(temp * pulse);
//           pulse = pulse / 58;
//           sum+=pulse;
//          // UARTprintf("Sum:%d\n",sum);
//       }
//      counts+=1;
//      if(sum>0)
//          return 0;
//   }
// return 1;
//}

