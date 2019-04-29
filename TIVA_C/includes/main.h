
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

//#include "i2cm_drv.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/debug.h"
#include "drivers/pinout.h"
#include "driverlib/fpu.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/queue.h"
#include "FreeRTOS/include/semphr.h"
#include "FreeRTOSConfig.h"

#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "utils/uartstdio.h"
//#include <sys/time.h>;
#include "inc/tm4c1294ncpdt.h"
 #include "driverlib/adc.h"
//#include "i2cm_drv.h"


//***********************************************************************************
//                                  Macros
//***********************************************************************************
#define YES 0
#define NO  1

#define TIVA_C  YES
#define COMMAND_SIZE (12)
#define DEV_ID      (0x00000000)
#define ON      (1)
#define OFF     (0)
#define CMD_CMOS_LED        (0x12)
#define CMD_OPEN            (0x00)
#define CMD_ENROLL_CNT      (0X20)
#define CMD_CHECK_ENR       (0x21)
#define CMD_ENROLL_START    (0x22)
#define CMD_CAPTURE_FIN     (0X60)
#define CMD_ENROLL_1        (0X23)
#define CMD_ENROLL_2        (0X24)
#define CMD_ENROLL_3        (0X25)
#define CMD_IS_PRESS        (0X26)
#define CMD_GET_IMAGE       (0X62)
#define CMD_DEL_ALL         (0X41)
#define CMD_VERIFY          (0x50)
#define CMD_ID              (0x51)
#define CMD_ENROLL_COUNT    (0x20)


#define PORTA   GPIO_PORTA_BASE
#define PORTB   GPIO_PORTB_BASE
#define PORTC   GPIO_PORTC_BASE
#define PORTD   GPIO_PORTD_BASE
#define PORTE   GPIO_PORTE_BASE
#define PORTF   GPIO_PORTF_BASE
#define PORTG   GPIO_PORTG_BASE
#define PORTH   GPIO_PORTH_BASE
#define PORTL   GPIO_PORTL_BASE

#define PIN0    GPIO_PIN_0
#define PIN1    GPIO_PIN_1
#define PIN2    GPIO_PIN_2
#define PIN3    GPIO_PIN_3
#define PIN4    GPIO_PIN_4
#define PIN5    GPIO_PIN_5
#define PIN6    GPIO_PIN_6
#define PIN7    GPIO_PIN_7

#define LED     0x01
#define TEMP    0X02

#define SYSTEM_CLOCK    120000000U


//***********************************************************************************
//                              Global variables
//***********************************************************************************


//***********************************************************************************
//                              Function Prototype
//***********************************************************************************

void FingerPrintInit(void);
void CommandSend(uint8_t param,uint8_t cmd_code);
uint32_t CommandResponse(uint8_t param);
void CheckFingerPrint(void);
uint16_t CheckSumCal(uint8_t* Command);
void AddFingerPrint(void);

void inputInt();
void Captureinit();
void InitConsole(void);
void ultrasonic_task(void *pvParameters);

//Function Prototypes
void demoLEDTask(void *pvParameters);
void i2c_init();
void temp_task(void *pvParameters);
void logger(void *pvParameters);
void __error__(char *pcFilename, uint32_t ui32Line);
void UART_Communication1(char *convert_structure1, int length);
void UART_Communication(char *convert_structure, int length);

void RelayInit(uint32_t Port,uint32_t Pin);     // (PORTH,2)
void RelayState(uint32_t PORT,uint32_t Pin,uint8_t state);
void servo_init(void);
void servo_pwm_config(void);
void UART_Communication2(char *convert_structure2, int length);
void fingerprint_task(void *pvParameters);
void servo(void);
void servo_alarm(void);
void startup_test_temperature();
void startup_test_ultrasonic();
void startup_test_fingerprint();
void startup();
void ultrasonic_value();
uint32_t ultrasonic_task1();
uint32_t CommandResponse(uint8_t param);
void CommandSend(uint8_t param,uint8_t cmd_code);
void startup();
double GetTime();
uint32_t CheckTest(void);
void SetUpADC(void);
uint32_t ADCConvert(void);
uint8_t SmokeStartUpTest(void);
void smoke_task(void *pvParameters);
void servo_alarm_off(void);
void servo_alarm(void);
void temp_func();




// Variables
extern uint8_t CommandArray[COMMAND_SIZE];
extern uint8_t ResponseArray[COMMAND_SIZE];

//uint32_t ui32SysClock;


typedef struct {
    char my_name[250];
    float temperature_value;
}structure_sent;

typedef struct {
    char ultra_name[250];
    //float ultra_distance;
}ultrasonic;

uint8_t temperature_data_celsius[30];
float Celsius;
char cThisChar;
int flag,flag1,flag2,flag_fingerprint;
uint32_t ui32Period;
//extern

typedef struct {
    char finger_name[250];
    //float ultra_distance;
}finger;

uint8_t check_id;
QueueHandle_t log_q,log_q1,log_q2;
int flag_startup;
uint32_t output_clock_rate_hz;
uint32_t count_global;


uint32_t pui32ADC0Value[1];
       uint8_t smoke_test_flag;
       uint8_t ultrasonic_test_flag;
       typedef struct {
           char my_name[250];
       }smoke_struct;
//////////
#define u (char)'U' //ultrasonic fail
#define f (char)'F' //smoke
#define h (char)'H' //temp
