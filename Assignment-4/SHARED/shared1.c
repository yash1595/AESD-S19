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

#define shared_memory_name 	"my_sm"
#define semaphore_name 		"my_semaphore"
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


	printf("Starting the process 2\n");
	int sm_fd = shm_open(shared_memory_name, O_CREAT | O_RDWR, 0666);		
	if(sm_fd < 0 )
	{
		perror("Cannot access shared memory");
		exit(1);
	}

	int sm_size = sizeof(STRUCT);

	void *shared_mem = mmap(NULL, sm_size, PROT_READ | PROT_WRITE, MAP_SHARED , sm_fd, 0);	
	if(shared_mem == (void *)-1)
	{
		perror("Mapping error");
		exit(1);
	}

	sem_t * sem = sem_open(semaphore_name, O_CREAT, 0666, 0);		
	if(sem == NULL)
	{
		perror("Semaphore error");
		exit(1);
	}

	sem_wait(sem);

		STRUCT Receive = {0};
        STRUCT *ptr = &Receive;

        memcpy(ptr,shared_mem, sizeof(STRUCT));	

        //Read incoming data			

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
        

		//Send Data
        //Sent.x = getpid();
        STRUCT Sent;
		strcpy(Sent.str,sp);
		strcpy(Sent.str1,sp1);
		strcpy(Sent.str2,sp2);
		strcpy(Sent.str3,sp3);
		strcpy(Sent.str4,sp4);
		Sent.str_length = strlen(Sent.str);
		Sent.status_Led = on_state;
		STRUCT*  ptr1 = &Sent;
		fp = fopen("shared.txt","a");
        clock_gettime(CLOCK_REALTIME, &t);
        fprintf(fp,"\nFIRST STRING : %s\n",Sent.str);
        fprintf(fp,"\nSTRING LENGTH : %d\n",Sent.str_length);
        fprintf(fp,"\nLED STATE : %d\n",Sent.status_Led);
        fprintf(fp,"\nSECOND STRING : %s\n",Sent.str1);
        fprintf(fp,"\nTHIRD STRING : %s\n",Sent.str2);
        fprintf(fp,"\nFOURTH STRING : %s\n",Sent.str3);
        fprintf(fp,"\nFIFTH STRING : %s\n",Sent.str4);
        fprintf(fp,"\nPID-1 : %d\n",Sent.x);
        fprintf(fp,"\nPID-2 : %d\n",Sent.x);
        fprintf(fp,"\nPID-3: %d\n",Sent.x);
        fprintf(fp,"\nTIMESTAMP CHILD TO PARENT: %ld \n", t.tv_nsec);
        fclose(fp);

		memcpy((char*)shared_mem, ptr1, sm_size);				//copy from memory

    	sem_post(sem);

	close(sm_fd);
  	sem_unlink(semaphore_name);
	return 0;
}
