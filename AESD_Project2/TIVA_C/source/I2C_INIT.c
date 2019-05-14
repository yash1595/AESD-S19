/*
 * I2C_INIT.c
 *
 *  Created on: Apr 10, 2019
 *      Author: mentor
 */


//#include "includes.h"
#include "main.h"
void i2c_init()
{
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
       GPIOPinConfigure(GPIO_PL1_I2C2SCL); //SCL pin
       GPIOPinConfigure(GPIO_PL0_I2C2SDA); //SDA Pins
       GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
       GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
       ROM_I2CMasterInitExpClk(I2C2_BASE,SYSTEM_CLOCK , false);
}
