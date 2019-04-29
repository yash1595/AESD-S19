
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
int flag = 0,fd,flag1=0,flag2=0;
char send1[5] = {'a'}; //temp
char send2[5] = {'n'};//temp
char send3[4] = {'d'};//distance
char send4[4] = {'o'};//distance
char send5[4] = {'h'};//smoke
char send6[4] = {'t'};//smoke
char send7[4] = {'x'};//smoke
struct itimerspec timer_setting;
struct sigevent signal_specification;
timer_t heartbeat_timer_id;

uint8_t READ_NOT_REC = 1;

void timer_check(int temp_sig)
{
    if(READ_NOT_REC==1)
    {
      printf("broken\n");
      ft4 = fopen("log_send.txt","a");
      fprintf(ft4,"******Broken BBG RX/TIVA TX*******\n");
      fclose(ft4);
    }
    else
    {
      printf("alive\n");
      ft4 = fopen("log_send.txt","a");
      fprintf(ft4,"******Alive Communication*******\n");
      fclose(ft4);
    }
    READ_NOT_REC=1;
}

void TimerInit(void)
{
    signal_specification.sigev_notify = SIGEV_THREAD;
    signal_specification.sigev_notify_function = &timer_check;
    signal_specification.sigev_value.sival_ptr = "Heartbeat timer";
    timer_create(CLOCK_REALTIME,&signal_specification,&heartbeat_timer_id);
    timer_setting.it_value.tv_sec =1;
    timer_setting.it_value.tv_nsec = 0;
    timer_setting.it_interval.tv_sec =6 ;
    timer_setting.it_interval.tv_nsec = 0;
    timer_settime(heartbeat_timer_id,0,&timer_setting,NULL);
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
    fclose(ft5);

    while(1)
    {
      //printf("Here\n");
        read(fd,&arr,sizeof(arr));
        READ_NOT_REC = 0;
        if(arr[0]=='T' || arr[0]=='D' || arr[0]=='S')
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
        }
        else if(arr[0]=='X')
        {
          ft5 = fopen("log_send.txt","a");
          fprintf(ft5,"******Broken BBG TX/TIVA RX*******\n");
          fclose(ft5);
        }
        else if(arr[0]=='L')
        {
          ft6 = fopen("log_send.txt","a");
          fprintf(ft6,"\n*****Startup Failed, System Exit*****\n");
          fclose(ft6);
        }
        else if(arr[0]=='M')
        {
          ft8 = fopen("log_send.txt","a");
          fprintf(ft8,"\n*****Startup Success*****\n");
          fclose(ft8);
        }
        else if(arr[0]=='H')
        {
          ft9 = fopen("log_send.txt","a");
          fprintf(ft9,"\n*****Temperature Fail******\n");
          fclose(ft9);
          write(fd,&send7, sizeof(send7));

        }
        else if(arr[0]=='U')
        {
          ft10 = fopen("log_send.txt","a");
          fprintf(ft10,"\n******Ultrasonic Fail*****\n");
          fclose(ft10);
          write(fd,&send7, sizeof(send7));

        }
        else if(arr[0]=='F')
        {
          ft10 = fopen("log_send.txt","a");
          fprintf(ft10,"\n******Smoke Fail*****\n");
          fclose(ft10);
          write(fd,&send7, sizeof(send7));

        }

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
   fprintf(ft2,"\n[Send]Common_ID : %d\n",my_ptr1->ID);
   fprintf(ft2,"\n[Send]Data is : %d\n",my_ptr1->data);
   fclose(ft2);
   printf("Data written\n");
  if(temp_val > 26)
  {
    ft2 = fopen("log_send.txt","a");
    fprintf(ft2,"Alert from Temperature*****\n");
    fclose(ft2);
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
   fprintf(ft12,"\n[Send]Common_ID : %d\n",my_ptr1->ID);
   fprintf(ft12,"\n[Send]Data is : %d\n",my_ptr1->data);
   fclose(ft12);
   printf("Data written\n");
  if(smoke_val <10 || smoke_val > 23)
  {
    printf("Alert is set\n");
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
   fprintf(ft3,"\n[Send]Common_ID : %d\n",my_ptr1->ID);
   fprintf(ft3,"\n[Send]Data is : %d\n",my_ptr1->data);
   fclose(ft3);
   if(distance_val < 5 || distance_val > 100  )
  {
    printf("Alert is set\n");
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

int main()

{

  TimerInit();
	uart_setup();

//	printf("Value of flag :%d\n",flag);

}
