#include "main.h"

void startup_test_temperature()
{
    //temp
    Temperature_Final_Value(&Celsius);
    if(Celsius == 0)
    {
     UARTprintf("Error in Temp\n");
     flag_startup = 1;
    }
    else
    {
     flag_startup = 0;
    }

}




//void startup_test_fingerprint()
//{
//   // CommandSend(1,CMD_CMOS_LED);
//   // CommandResponse(CMD_CMOS_LED);
//
////    if(flag_fingerprint == 0)
////    {
////        flag_startup =1;
////        UARTprintf("Alert from fingerprint\n");
////    }
////    else
////    {
////        flag_startup = 0;
////    }
//}



void startup()
{
    smoke_test_flag=0;
    startup_test_temperature();
    smoke_test_flag = SmokeStartUpTest();
    flag_startup = CheckTest();
    UARTprintf("Startup\n");

    char l='L';
    char m='M';
    if(flag_startup == 1 || smoke_test_flag==1)
    {
        UARTprintf("\n****Exiting the system, Startup Failed****\n");
        UARTCharPut(UART3_BASE,l);
        UARTCharPut(UART0_BASE,l);
    }
    else
    {
        UARTprintf("\n****Startup Success, Starting System****\n");
        UARTCharPut(UART3_BASE,m);
        UARTCharPut(UART0_BASE,m);
    }

}
