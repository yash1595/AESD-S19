/*
 * relay.h
 *
 *  Created on: Apr 18, 2019
 *      Author: yashm
 */

#ifndef SRC_RELAY_H_
#define SRC_RELAY_H_

#include "main.h"
#include "src/finger_print.h"

#include <stdint.h>
#include <stdbool.h>
#include "drivers/pinout.h"
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

#include "i2cm_drv.h"
#include "driverlib/i2c.h"
#include "inc/hw_i2c.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define PORTA   GPIO_PORTA_BASE
#define PORTB   GPIO_PORTB_BASE
#define PORTC   GPIO_PORTC_BASE
#define PORTD   GPIO_PORTD_BASE
#define PORTE   GPIO_PORTE_BASE
#define PORTF   GPIO_PORTF_BASE
#define PORTG   GPIO_PORTG_BASE
#define PORTH   GPIO_PORTH_BASE

#define PIN0    GPIO_PIN_0
#define PIN1    GPIO_PIN_1
#define PIN2    GPIO_PIN_2
#define PIN3    GPIO_PIN_3
#define PIN4    GPIO_PIN_4
#define PIN5    GPIO_PIN_5
#define PIN6    GPIO_PIN_6
#define PIN7    GPIO_PIN_7


void RelayInit(uint32_t Port,uint32_t Pin);     // (PORTH,2)
void RelayState(uint32_t PORT,uint32_t Pin,uint8_t state);



#endif /* SRC_RELAY_H_ */
