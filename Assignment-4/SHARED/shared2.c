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

#define shared_memory_name  "my_sm"
#define semaphore_name    "my_semaphore"
#define on_state  1;
#define off_state  0;
char* FILE_NAME = "shared.txt";
char* message = "Terminated by SIGTERM\n";

volatile sig_atomic_t done = 0;
struct timespec t;
FILE *fp;
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
    fprintf(fp,"%s Using Semaphores as resources implementation via Shared Memory",message);
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
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

  printf("Currently in process 1\n");

	int sm_fd = shm_open(shared_memory_name, O_CREAT | O_RDWR, 0666);	//create shared memory
	if(sm_fd < 0)
	{
		perror("Cannot access shared memory");
		exit(1);
	}

	int sm_size = sizeof(STRUCT);

	int value = ftruncate(sm_fd, sm_size);
	if(value < 0)
	{
		perror("Ftruncate error");
		exit(1);
	}

	STRUCT * shared_mem = mmap(NULL, sm_size, PROT_READ | PROT_WRITE, MAP_SHARED , sm_fd, 0);	//memory mapping
	if(shared_mem < 0)
	{
		perror("Mapping error");
		exit(1);
	}

	sem_t * sem = sem_open(semaphore_name, O_CREAT, 0666, 0);			//open semaphore
	if(sem == NULL)
	{
		perror("Semaphore error");
		exit(1);
	}


    Sent.x = getpid();

    //Parent Sends
    strcpy(Sent.str,sc);
    strcpy(Sent.str1,sc1);
    strcpy(Sent.str2,sc2);
    strcpy(Sent.str3,sc3);
    strcpy(Sent.str4,sc4);
    Sent.str_length = strlen(Sent.str);
    Sent.status_Led = on_state;
    STRUCT *ptr = &Sent;
    FILE *fp;
    fp = fopen(FILE_NAME,"a");

    clock_gettime(CLOCK_REALTIME, &t);
    fprintf(fp,"\nUsing Semaphores as resources implementation via Shared Memory\n");
    fprintf(fp,"\nFIRST STRING : %s\n",ptr->str);
    fprintf(fp,"\nSTRING LENGTH : %d\n",ptr->str_length);
    fprintf(fp,"\nLED STATE : %d\n",ptr->status_Led);
    fprintf(fp,"\nSECOND STRING : %s\n",ptr->str1);
    fprintf(fp,"\nTHIRD STRING : %s\n",ptr->str2);
    fprintf(fp,"\nFOURTH STRING : %s\n",ptr->str3);
    fprintf(fp,"\nFIFTH STRING : %s\n",ptr->str4);
    fprintf(fp,"\nPID-1 : %d\n",ptr->x);
    fprintf(fp,"\nPID-2 : %d\n",ptr->x);
    fprintf(fp,"\nPID-3: %d\n",ptr->x);
    fprintf(fp,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
    fclose(fp);


    memcpy(shared_mem, &Sent, sm_size);

    sem_post(sem);

    //Read STRUCT

  	STRUCT Receive;

  	memcpy(&Receive, shared_mem, sm_size);
    STRUCT* ptr1 = &Receive;
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

	// printf("Message from process 2\n");
  //   	printf("Message: %s\nMessageLen: %d\nUSRLED: %d\n",Receive.str,Receive.str_length,Receive.status_Led);

	value = shm_unlink(shared_memory_name);			//unlink shared memory
	if(value < 0)
	{
		perror("Unlink error");
		exit(1);
	}
    printf("here");

	return 0;
}
