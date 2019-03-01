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
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define on_state  1;
#define off_state  0;

char* FILE_NAME = "socket.txt";
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
    fprintf(fp,"%s Using no resources implementation via Sockets",message);
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

#define PORT 8080

void main()
{

  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGTERM, &action, NULL);
	int sock;
	struct sockaddr_in server, client;
	int mysock;
	char buff[1024];
	int rval;

	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Failed to create a socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5003);

	//bind
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Didn't bind");
		exit(1);

	}

	//Listen
	if(listen(sock, 5) < 0)
	{
		perror("Listening error");
		exit(1);
	}

	//Accept
	mysock = accept(sock, (struct sockaddr *)0, 0);
	if(mysock == -1)
	{
		perror("accept failed");
		exit(1);
	}
  STRUCT Received_STRUCT = {0};
STRUCT *	ptr = &Received_STRUCT;

	int STRUCT_in = read(mysock,&Received_STRUCT,sizeof(STRUCT));

	if (STRUCT_in < 0)
	{
		perror("Error reading");
		exit(1);
	}
  fp1 = fopen("socket.txt","a");
  clock_gettime(CLOCK_REALTIME, &t);
   fprintf(fp1,"\nUsing queues as resources implementation via Sockets\n");
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
  //
	// ptr = (mystruct_t*)(buff);
	// printf("Hello from server\n");
  //       printf("Message: %s, Message length = %d, LED: %d\n", ptr->my_str, ptr->str_length, ptr->status_Led);






  Sent.x = getpid();

  strcpy(Sent.str,sp);
  strcpy(Sent.str1,sp1);
  strcpy(Sent.str2,sp2);
  strcpy(Sent.str3,sp3);
  strcpy(Sent.str4,sp4);
  Sent.str_length = strlen(Sent.str);
  Sent.status_Led = on_state;
  STRUCT*  ptr1 = &Sent;
  fp1 = fopen("socket.txt","a");
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

    	int STRUCT_out = write(mysock,&Sent,sizeof(STRUCT));
	if (STRUCT_out < 0)
	{
		perror("Error writing");
		exit(1);
	}
	exit(1);
}
