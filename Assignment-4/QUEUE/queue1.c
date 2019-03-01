#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdlib.h>

#define name 		"/queue"
#define size		8
#define on_state  1;
#define off_state  0;

char* FILE_NAME = "queue.txt";

struct timespec t;
FILE *fp1,*fp;
char *message = "Terminated by SIGTERM\n";
int done = 0;
void term(int signum)
{
    char buffer[30];
    struct timeval tv;

    time_t curtime;
    done = 1;
    printf("%s",message);
    fopen(FILE_NAME,"a+");
    gettimeofday(&tv, NULL); 
    curtime=tv.tv_sec;
    strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
    fprintf(fp,"%s%ld\n",buffer,tv.tv_usec);
    fprintf(fp,"%s Using Semaphores as resources implementation via POSIX queue",message);
    fclose(fp);
    exit(0);
}

typedef struct
{
    char str[50],str1[50],str2[50],str3[50],str4[50];
    int str_length, status_Led;
    int x ;
    long int leds1;
    long int leds2; 
}STRUCT;
STRUCT Sent;

char sp[50]="\nSecond str\n";
char sp1[50]="\nThird str\n";
char sp2[50]="\nFourth str\n";
char sp3[50]="\nFifth str\n";
char sp4[50]="\nSixth str\n";

char sc[50] = "\nOne\n";
char sc1[50]="\nTwo\n";
char sc2[50]="\nThree\n";
char sc3[50]="\nFour\n";
char sc4[50]="\nFive\n";

int main()
{
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term; //
  sigaction(SIGTERM, &action, NULL);
	mqd_t myqueue;

	struct mq_attr myqueue_attr;
	myqueue_attr.mq_maxmsg = size;
	myqueue_attr.mq_msgsize = sizeof(STRUCT);

	myqueue = mq_open(name, O_CREAT | O_RDWR, 0666, &myqueue_attr);		//open queue

	if(myqueue == (mqd_t)-1)
	{
		perror("Queue opening error");
		exit(1);
	}

	//char msg[] = "Hello from Process1";
    	STRUCT Sent;




      Sent.x = getpid();

      //STRUCT Sent ;
      strcpy(Sent.str,sp);
      strcpy(Sent.str1,sp1);
      strcpy(Sent.str2,sp2);
      strcpy(Sent.str3,sp3);
      strcpy(Sent.str4,sp4);
      Sent.str_length = strlen(Sent.str);
      Sent.status_Led = on_state;
      STRUCT *ptr = &Sent;
      fp1 = fopen("queue.txt","a");
      clock_gettime(CLOCK_REALTIME, &t);
      fprintf(fp1,"\nUsing Semaphores as resources implementation via POSIX queue\n");
      fprintf(fp1,"\nFIRST STRING : %s\n",ptr->str);
      fprintf(fp1,"\nSTRING LENGTH : %d\n",ptr->str_length);
      fprintf(fp1,"\nLED STATE : %d\n",ptr->status_Led);
      fprintf(fp1,"\nSECOND STRING : %s\n",ptr->str1);
      fprintf(fp1,"\nTHIRD STRING : %s\n",ptr->str2);
      fprintf(fp1,"\nFOURTH STRING : %s\n",ptr->str3);
      fprintf(fp1,"\nFIFTH STRING : %s\n",ptr->str4);
      fprintf(fp1,"\nPID-1 : %d\n",ptr->x);
      fprintf(fp1,"\nPID-2 : %d\n",ptr->x);
      fprintf(fp1,"\nPID-3: %d\n",ptr->x);
      fprintf(fp1,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
      fclose(fp1);


	int ret = mq_send(myqueue,&Sent, sizeof(STRUCT),0);		//send message
	if(ret == -1)
	{
		perror("Send error");
		exit(1);
	}
	printf("Message sent to process2 is:\n");
	//printf("Message: %s\nMessage Length: %d\nLED: %d\n",Sent.str_length,Sent.str_length,Sent.status_Led);
	printf("\n");

  STRUCT Receive;

	ret = mq_receive(myqueue, &Receive, sizeof(STRUCT), 0);	//receive message
	if(ret == -1)
	{
		perror("Receive error");
		exit(1);
	}
  STRUCT* ptr1 = &Receive;
  fp1 = fopen("queue.txt","a");
  clock_gettime(CLOCK_REALTIME, &t);
  fprintf(fp1,"\nFIRST STRING : %s\n",ptr1->str);
  fprintf(fp1,"\nSTRING LENGTH : %d\n",ptr1->str_length);
  fprintf(fp1,"\nLED STATE : %d\n",ptr1->status_Led);
  fprintf(fp1,"\nSECOND STRING : %s\n",ptr1->str1);
  fprintf(fp1,"\nTHIRD STRING : %s\n",ptr1->str2);
  fprintf(fp1,"\nFOURTH STRING : %s\n",ptr1->str3);
  fprintf(fp1,"\nFIFTH STRING : %s\n",ptr1->str4);
  fprintf(fp1,"\nPID-1 : %d\n",ptr1->x);
  fprintf(fp1,"\nPID-2 : %d\n",ptr1->x);
  fprintf(fp1,"\nPID-3: %d\n",ptr1->x);
  fprintf(fp1,"\nTIMESTAMP CHILD TO PARENT: %ld \n", t.tv_nsec);
  fclose(fp1);

	// printf("Message received from process2 is:\n");
  //   	printf("Message: %s\nMessageLen: %d\nLED: %d\n",Receive.str_length,Receive.str_length,Receive.status_Led);
	// printf("\n");

	mq_close(myqueue);
	mq_unlink(name);								//close queue
	return 0;
}
