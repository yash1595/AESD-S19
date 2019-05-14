
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/shm.h>
FILE *ft2,*ft3,*ft4,*ft5,*ft6,*ft7,*ft8,*ft9,*ft10,*ft11,*ft12,*ft13;
//Variables defined
char * uart_driver = "/dev/ttyO2";
struct termios *my_term;
void termios_setup(struct termios * my_term, int descriptor);
char arr[20],temp[10],temp1[20];
int flag = 0,fd,flag1=0,flag2=0,flag4=0;
char send1[5] = {'a'}; //temp
char send2[5] = {'n'};//temp
char send3[3] = {'d'};//distance
char send4[3] = {'o'};//distance
char send5[3] = {'h'};//smoke
char send6[3] = {'t'};//smoke
char send7[3] = {'x'};//sensor fail
char send8[3] = {'y'}; //communication of TIVA TXx fail
char send9[3] = {'z'}; //Alert from any sensor
char send10[3] = {'r'}; //Fingerprint
char send11[3] = {'s'}; //Fingerprint

struct itimerspec timer_setting;
struct sigevent signal_specification;
timer_t continuous_check_id;

uint8_t READ_NOT_REC = 1;

void timer_check(int temp_sig)
{
    if(READ_NOT_REC==1)
    {
      printf("broken\n");
      ft4 = fopen("log_send.txt","a");
      fprintf(ft4,"[%lf]******Broken BBG RX/TIVA TX*******\n",GetTime());
      fclose(ft4);
      write(fd,&send8, sizeof(send8));
      alloff();
      func2();

    }
    else
    {
      printf("alive\n");
      ft4 = fopen("log_send.txt","a");
      fprintf(ft4,"[%lf]******Alive Communication*******\n",GetTime());
      fclose(ft4);
    }
    READ_NOT_REC=1;
}

void TimerInit(void)
{
    signal_specification.sigev_notify = SIGEV_THREAD;
    signal_specification.sigev_notify_function = &timer_check;
    signal_specification.sigev_value.sival_ptr = "timer";
    timer_create(CLOCK_REALTIME,&signal_specification,&continuous_check_id);
    timer_setting.it_value.tv_sec =1;
    timer_setting.it_value.tv_nsec = 0;
    timer_setting.it_interval.tv_sec =6 ;
    timer_setting.it_interval.tv_nsec = 0;
    timer_settime(continuous_check_id,0,&timer_setting,NULL);
}


void uart_setup()				//uart driver setup
{
    fd = open(uart_driver, O_RDWR, O_SYNC, O_NOCTTY);
    if(fd < 0)
    {
    	perror("Error opening uart driver");
    }

    my_term  = (struct termios *)malloc(sizeof(struct termios));

    termios_setup(my_term, fd);
}



typedef struct
{
  //char name[30];
  int ID;
  int data;

}populated_structure;

populated_structure data_to_sent;
populated_structure  *my_ptr1 = &data_to_sent;

void termios_setup(struct termios * my_term, int descriptor)
{
    tcgetattr(descriptor, my_term);
    my_term->c_iflag &= ~(IGNBRK | ICRNL | INLCR | PARMRK | ISTRIP | IXON);
    my_term->c_oflag &= ~OPOST;
    my_term->c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    my_term->c_cflag |= CS8 | CLOCAL;

    cfsetispeed(my_term, B115200);
    cfsetospeed(my_term, B115200);

    if(tcsetattr(descriptor, TCSAFLUSH, my_term) < 0)
    {
        perror("ERROR in set attr\n");
    }
    ft5 = fopen("log_send.txt","a");
    fprintf(ft5,"******Menu*******\n");
    fprintf(ft5,"ID =1, Temperature Sensor*******\n");
    fprintf(ft5,"ID =2, Ultrasonic Sensor*******\n");
    fprintf(ft5,"ID =3, Smoke Sensor*******\n");
    fprintf(ft5,"ID =4, Fingerprint Sensor*******\n");
    fprintf(ft5,"******Seven Segment ID = 1 Alert from Sensor*******\n");
    fprintf(ft5,"******Seven Segment ID = 2 Alert from Communication*******\n");
    fprintf(ft5,"******Seven Segment ID = 3 Alert from Sensor CutOff*******\n");
    fprintf(ft5,"******Seven Segment ID = 4 Alert from Startup fail*******\n");
    fclose(ft5);

    while(1)
    {
      //printf("Here\n");
        read(fd,&arr,sizeof(arr));
        READ_NOT_REC = 0;
        if(arr[0]=='T' || arr[0]=='D' || arr[0]=='S' || arr[0]=='P')
        {
            if((arr[0])=='T')
            {
             data_to_sent.ID = 1;
             printf("T is checked\n");
             read_temp();
            }
            else if(arr[0]=='D')
            {
             printf("D is checked\n");
             read_distance();
            }
            else if(arr[0]=='S')
            {
             printf("S is checked\n");
             read_smoke();
            }
            else if(arr[0]=='P')
            {
             printf("P is checked\n");
             read_finger();
            }
        }
        else if(arr[0]=='X')
        {
          ft5 = fopen("log_send.txt","a");
          fprintf(ft5,"[%lf]******Broken BBG TX/TIVA RX*******\n",GetTime());
          fclose(ft5);
          alloff();
          func8();
        //  sleep(1);
          //alloff();
        }
        else if(arr[0]=='L')
        {
          ft6 = fopen("log_send.txt","a");
          fprintf(ft6,"\n[%lf]*****Startup Failed, System Exit*****\n",GetTime());
          fclose(ft6);
          alloff();
          func4();
        }
        else if(arr[0]=='M')
        {
          ft8 = fopen("log_send.txt","a");
          fprintf(ft8,"\n[%lf]*****Startup Success*****\n",GetTime());
          fclose(ft8);
        }
        else if(arr[0]=='H')
        {
          ft9 = fopen("log_send.txt","a");
          fprintf(ft9,"\n[%lf]*****Temperature Fail******\n",GetTime());
          fclose(ft9);
          write(fd,&send7, sizeof(send7));
          alloff();
          func3();

        }
        else if(arr[0]=='U')
        {
          ft10 = fopen("log_send.txt","a");
          fprintf(ft10,"\n[%lf]******Ultrasonic Fail*****\n",GetTime());
          fclose(ft10);
          write(fd,&send7, sizeof(send7));
          alloff();
          func3();

        }
        else if(arr[0]=='F')
        {
          ft10 = fopen("log_send.txt","a");
          fprintf(ft10,"\n[%lf]******Smoke Fail*****\n",GetTime());
          fclose(ft10);
          write(fd,&send7, sizeof(send7));
          alloff();
          func3();

        }

    }

}

void read_finger(void)
{
  data_to_sent.ID = 4;
  uint8_t p_data[5];
  int i = 1;
  while(arr[i]!='P')
  {
    p_data[i-1]=(arr[i]);
    printf("p_data :%c",p_data[i-1]);
    i+=1;
    printf("Value of i: %d",i);
  }

  uint8_t finger_val = atoi(p_data);
  printf("Final Finger value %d\n",finger_val);
  data_to_sent.data = finger_val;
  printf("Before opening a file\n");
  ft2 = fopen("log_send.txt","a");
   fprintf(ft2,"\n[%lf][Send]Common_ID : %d\n",GetTime(),my_ptr1->ID);
   fprintf(ft2,"\n[%lf][Send]Data is : %d\n",GetTime(),my_ptr1->data);
   fclose(ft2);
   printf("Data written\n");
  if(finger_val != 20 )
  {
    ft2 = fopen("log_send.txt","a");
    fprintf(ft2,"[%lf]Alert from Finger*****\n",GetTime());
    fclose(ft2);
    write(fd,&send9, sizeof(send9));
    alloff();
    func1();
    flag4 = 1;
  }
  if(flag4 == 1)
  {
   write(fd,&send10, sizeof(send10));
   printf("%s\n",send10);
   flag4 =0;
  }
  else
  {
   write(fd,&send11, sizeof(send11));
   printf("%s\n",send11);

  }

}
void read_temp(void)
{
  //uint8_t temp_value_final[5];
  uint8_t t_data[5];
  int i = 1;
  while(arr[i]!='T')
  {
    t_data[i-1]=(arr[i]);
    printf("t_data :%c",t_data[i-1]);
    i+=1;
    printf("Value of i: %d",i);
  }

  uint8_t temp_val = atoi(t_data);
  printf("Final Temp value %d\n",temp_val);
  data_to_sent.data = temp_val;
  printf("Before opening a file\n");
  ft2 = fopen("log_send.txt","a");
   fprintf(ft2,"\n[%lf][Send]Common_ID : %d\n",GetTime(),my_ptr1->ID);
   fprintf(ft2,"\n[%lf][Send]Data is : %d\n",GetTime(),my_ptr1->data);
   fclose(ft2);
   printf("Data written\n");
  if(temp_val > 26)
  {
    ft2 = fopen("log_send.txt","a");
    fprintf(ft2,"[%lf]Alert from Temperature*****\n",GetTime());
    fclose(ft2);
    write(fd,&send9, sizeof(send9));
    alloff();
    func1();
    flag = 1;
  }
  if(flag == 1)
  {
   write(fd,&send1, sizeof(send1));
   printf("%s\n",send1);
   flag =0;
  }
  else
  {
   write(fd,&send2, sizeof(send2));
   printf("%s\n",send2);

  }
}

void read_smoke(void)
{
  data_to_sent.ID = 3;

  //uint8_t temp_value_final[5];
  uint8_t s_data[5];
  int i = 1;
  while(arr[i]!='S')
  {
    s_data[i-1]=(arr[i]);
    printf("t_data :%c",s_data[i-1]);
    i+=1;
    printf("Value of i: %d",i);
  }

  uint8_t smoke_val = atoi(s_data);
  printf("Final Smoke value %d\n",smoke_val);
  data_to_sent.data = smoke_val;
  ft12 = fopen("log_send.txt","a");
   fprintf(ft12,"\n[%lf][Send]Common_ID : %d\n",GetTime(),my_ptr1->ID);
   fprintf(ft12,"\n[%lf][Send]Data is : %d\n",GetTime(),my_ptr1->data);
   fclose(ft12);
   printf("Data written\n");
  if(smoke_val <10 || smoke_val > 27)
  {
    ft2 = fopen("log_send.txt","a");
    fprintf(ft2,"[%lf]Alert from Smoke*****\n",GetTime());
    fclose(ft2);
    write(fd,&send9, sizeof(send9));
    alloff();
    func1();
    flag2= 1;
  }
  if(flag2 == 1)
  {
   write(fd,&send5, sizeof(send5));
   printf("%s\n",send5);
   flag2 =0;
  }
  else
  {
   write(fd,&send6, sizeof(send6));
   printf("%s\n",send6);

  }
}


void read_distance(void)
{
  data_to_sent.ID = 2;
  //uint8_t temp_value_final[5];
  uint8_t d_data[5];
  int i = 1;
  while(arr[i]!='D')
  {
    d_data[i-1]=(arr[i]);
    printf("d_data :%c",d_data[i-1]);
    i+=1;
    printf("Value of i: %d",i);
  }

  uint8_t distance_val = atoi(d_data);
  printf("Final Distance value %d\n",distance_val);
  data_to_sent.data = distance_val;
  ft3 = fopen("log_send.txt","a");
   fprintf(ft3,"\n[%lf][Send]Common_ID : %d\n",GetTime(),my_ptr1->ID);
   fprintf(ft3,"\n[%lf][Send]Data is : %d\n",GetTime(),my_ptr1->data);
   fclose(ft3);
   if(distance_val < 5 || distance_val > 100  )
  {
    ft2 = fopen("log_send.txt","a");
    fprintf(ft2,"[%lf]Alert from Distance*****\n",GetTime());
    fclose(ft2);
    write(fd,&send9, sizeof(send9));
    alloff();
    func1();
    flag1 = 1;
  }
  if(flag1 == 1)
  {
   write(fd,&send3, sizeof(send3));
   printf("%s\n",send3);
   flag1 =0;
  }
  else
  {
   write(fd,&send4, sizeof(send4));
   printf("%s\n",send4);

  }
}


double GetTime()
{
	struct timeval time;

	gettimeofday(&time, 0);

	return (double)(time.tv_sec)+(((double)(time.tv_usec))/1000000);
}



//#include "main.h"

void func0()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	//gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func1()
{
	gpio_on_off(gpio49,1);
	// gpio_on_off(gpio66,1);
	// gpio_on_off(gpio67,1);
	// gpio_on_off(gpio68,1);
	// gpio_on_off(gpio69,1);
	// gpio_on_off(gpio44,1);
	// gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func2()
{
	//gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	//gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func3()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
//	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
//	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func4()
{
	gpio_on_off(gpio49,1);
	//gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
//	gpio_on_off(gpio44,1);
	//gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func5()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
//	gpio_on_off(gpio27,1);
//	gpio_on_off(gpio117,1);
}
void func6()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	//gpio_on_off(gpio117,1);
}
void func7()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	//gpio_on_off(gpio67,1);
//	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
//	gpio_on_off(gpio44,1);
//	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func8()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void func9()
{
	gpio_on_off(gpio49,1);
	gpio_on_off(gpio66,1);
	gpio_on_off(gpio67,1);
	gpio_on_off(gpio68,1);
	gpio_on_off(gpio69,1);
	gpio_on_off(gpio44,1);
	//gpio_on_off(gpio27,1);
	gpio_on_off(gpio117,1);
}
void alloff()
{
	gpio_on_off(gpio49,0);
	gpio_on_off(gpio66,0);
	gpio_on_off(gpio67,0);
	gpio_on_off(gpio68,0);
	gpio_on_off(gpio69,0);
	gpio_on_off(gpio44,0);
	gpio_on_off(gpio27,0);
	gpio_on_off(gpio117,0);
}



void gpio_on_off(uint8_t pin,uint8_t state)
{
	FILE *fp1;

	if(pin<8 && state<2)
	{
		fp1 = fopen(LED_Dir_Path[pin],"w");
		if(fp1==NULL) printf("Error in led\n");
		//else
	//	printf("\n Success file open");
	}
	else
	{
		printf("\n Error in pin or state\n");
		return ;
	}

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",pin);
	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%s","out");
	fclose(fp1);

	fp1 = 	fopen(LED_Val_Path[pin],"w");
	if(fp1==NULL) printf("Error in led\n");
	else
//	printf("\n Success file open 2");

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",state);

	fclose(fp1);

}
int main()

{
  alloff();
  TimerInit();
  printf("[%lf] [MAIN]::Successfully created State Machine!\n", GetTime());
	uart_setup();

//	printf("Value of flag :%d\n",flag);

}
