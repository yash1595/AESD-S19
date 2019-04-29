/*
 * TempTask.c
 *
 *  Created on: Apr 10, 2019
 *      Author: mentor
 */




#include "main.h"


void temp_task(void *pvParameters)
{



    char Text_Print[150];
    char str[10];
    int count=0;
    UARTprintf("\nTempTask Started\n");


      static structure_sent temp_info;

      while(1)
        {

              count_global+=1;
              if(count_global%10==0)
              {
                  UARTprintf("Count%d\n",count);
                  //timer_init();
                  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);    //Enables individual timer interrupt
                  count_global=0;
              }

          Temperature_Final_Value(&Celsius);
       // UARTprintf("Value%f",Celsius);
        if(Celsius<40)
        {
//            count = xTaskGetTickCount();
//                     count = count - 1;
//                    // UARTprintf("%d\n",count);
//                     if(count%10000==0)
//                     {
//                         strcpy(str,"A");
//                     }
//
//                     else
                     {
                       strcpy(str,"T");
                     }

        snprintf(Text_Print, sizeof(Text_Print), "%d",(int)Celsius);
        strcat(str,Text_Print);
        strcat(str,"T");
        strcpy(temp_info.my_name,str);
        bzero(str,sizeof(str));
        IntMasterEnable();
        IntEnable(INT_UART3);
        UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
        if(xQueueSend(log_q,(void *)&temp_info, 100) != pdTRUE)
        {
            UARTprintf("\nLog Error");
        }


         }
        else

        {
            UARTprintf("Temperature Failed\n");
            UARTCharPut(UART3_BASE,h);
        }
                vTaskDelay(1000);
        }

}

void Temperature_Final_Value(float *temp_final_Celsius)
{
    static structure_sent temp_info;
    ROM_I2CMasterSlaveAddrSet(I2C2_BASE, 0x48, true); //address of temp sensor
    ROM_I2CMasterDataPut(I2C2_BASE, 0x00); //in order to read from reg
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(!ROM_I2CMasterBusy(I2C2_BASE));
    while(ROM_I2CMasterBusy(I2C2_BASE));
    temperature_data_celsius[0] = (uint8_t)ROM_I2CMasterDataGet(I2C2_BASE);
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    temperature_data_celsius[1] = (uint8_t)ROM_I2CMasterDataGet(I2C2_BASE);
    ROM_I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    *temp_final_Celsius = (((temperature_data_celsius[0] << 8) | temperature_data_celsius[1]) >> 4) * 0.0625;
    temp_info.temperature_value = Celsius;
    float Temp_Fehrenheit = (*temp_final_Celsius * 1.8) + 32;
    float Temp_Kelvin = *temp_final_Celsius + 273.15;

}




