/*
 * relay.c
 *
 *  Created on: Apr 18, 2019
 *      Author: yashm
 */
#include "relay.h"

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
