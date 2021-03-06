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

#define queue_name 		"/queue"
#define queue_size		8
#define on_state  1;
#define off_state  0;

struct timespec t;
FILE *fp;
char* FILE_NAME = "queue.txt";
char *message = "Terminated by SIGTERM\n";

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
    fprintf(fp,"%s Using Semaphores as resources implementation via POSIX QUEUES",message);
    fclose(fp);
    exit(0);
}

int main()
{

	printf("Starting process\n");
	printf("\n");

  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term; //
  sigaction(SIGTERM, &action, NULL);

	mqd_t myqueue;
	struct mq_attr myqueue_attr;
	myqueue_attr.mq_maxmsg = queue_size;
	myqueue_attr.mq_msgsize = sizeof(STRUCT);

	myqueue = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &myqueue_attr);		//open queue
	if(myqueue == -1)
	{
		perror("Queue opening error");
		exit(1);
	}

	

  STRUCT Receive = {0};
  STRUCT*  ptr = &Receive;

  int ret = mq_receive(myqueue, ptr, sizeof(STRUCT),0);     //receive message
  if(ret == -1)
  {
    perror("Receiving error");
    exit(1);
  }

  fp = fopen("queue.txt","a");
  clock_gettime(CLOCK_REALTIME, &t);
  fprintf(fp,"\nUsing Semaphores as resources implementation via POSIX QUEUES\n");
  fprintf(fp,"\nFIRST str : %s\n",ptr->str);
  fprintf(fp,"\nstr LENGTH : %d\n",ptr->str_length);
  fprintf(fp,"\nLED STATE : %d\n",ptr->status_Led);
  fprintf(fp,"\nSECOND str : %s\n",ptr->str1);
  fprintf(fp,"\nTHIRD str : %s\n",ptr->str2);
  fprintf(fp,"\nFOURTH str : %s\n",ptr->str3);
  fprintf(fp,"\nFIFTH str : %s\n",ptr->str4);
  fprintf(fp,"\nPID-1 : %d\n",ptr->x);
  fprintf(fp,"\nPID-2 : %d\n",ptr->x);
  fprintf(fp,"\nPID-3: %d\n",ptr->x);
  fprintf(fp,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
  fclose(fp);



  Sent.x = getpid();

  //STRUCT Sent ;
  strcpy(Sent.str,sc);
  strcpy(Sent.str1,sc1);
  strcpy(Sent.str2,sc2);
  strcpy(Sent.str3,sc3);
  strcpy(Sent.str4,sc4);
  Sent.str_length = strlen(Sent.str);
  Sent.status_Led = on_state;
  STRUCT*  ptr1 = &Sent;
  fp = fopen(FILE_NAME,"a");
  clock_gettime(CLOCK_REALTIME, &t);
  fprintf(fp,"\nFIRST STRING : %s\n",ptr1->str);
  fprintf(fp,"\nSTRING LENGTH : %d\n",ptr1->str_length);
  fprintf(fp,"\nLED STATE : %d\n",ptr1->status_Led);
  fprintf(fp,"\nSECOND STRING : %s\n",ptr1->str1);
  fprintf(fp,"\nTHIRD STRING : %s\n",ptr1->str2);
  fprintf(fp,"\nFOURTH STRING : %s\n",ptr1->str3);
  fprintf(fp,"\nFIFTH STRING : %s\n",ptr1->str4);
  fprintf(fp,"\nPID-1 : %d\n",ptr1->x);
  fprintf(fp,"\nPID-2 : %d\n",ptr1->x);
  fprintf(fp,"\nPID-3: %d\n",ptr1->x);
  fprintf(fp,"\nTIMESTAMP CHILD TO PARENT: %ld \n", t.tv_nsec);
  fclose(fp);
	ret = mq_send(myqueue, &Sent, sizeof(STRUCT),0);			//send message
	if(ret == -1)
	{
		perror("Send error");
		exit(1);
	}

	mq_close(myqueue);
	mq_unlink(queue_name);								//close queue
	return 0;
}
