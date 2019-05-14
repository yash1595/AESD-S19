#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h> //creating p thread
#include<time.h> //Timestamp function
#include<malloc.h> //Space
#include<unistd.h>

#define on_state  1;
#define off_state  0;


FILE *fp;
char* FILE_NAME = "pipes.txt";
char* message = "Terminated by SIGTERM\n";


int parent[10];
int child[10];


struct timespec t;
int flag = 0;

typedef struct
{
    char str[50],str1[50],str2[50],str3[50],str4[50];
    int str_length, status_Led;
    int x ;
    long int leds1;
    long int leds2; 
}pipe_struct;

pipe_struct *pipe_led;
pipe_struct pipe_struct2;

/* SIGTERM */
volatile sig_atomic_t done = 0;
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
    fprintf(fp,"%s There were no acquired resources",message);
    fclose(fp);
    exit(0);
}

        char s1p[50]="\nSecond str\n";
        char s2p[50]="\nThird str\n";
        char s3p[50]="\nFourth str\n";
        char s4p[50]="\nFifth str\n";
        char s5p[50]="\nSixth str\n";

        char sc[50] = "\nOne\n";
       char s1c[50]="\nTwo\n";
       char s2c[50]="\nThree\n";
       char s3c[50]="\nFour\n";
       char s4c[50]="\nFive\n";

int main()
{

    
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);


    pid_t fork_process_used;
   
    if (pipe(parent)<0 || pipe(child)<0 )
    {
        printf("\nError\n");
        return 1;
    }

    fork_process_used = fork();

    if (fork_process_used < 0)
    {
        printf("\nError is there\n");
        return 1;
    }

    else if (fork_process_used > 0)
    {

      pipe_struct2.x = getpid();


       strcpy(pipe_struct2.str,sc);
       strcpy(pipe_struct2.str1,s1c);
       strcpy(pipe_struct2.str2,s2c);
       strcpy(pipe_struct2.str3,s3c);
       strcpy(pipe_struct2.str4,s4c);
       pipe_struct2.str_length = strlen(pipe_struct2.str);
       pipe_struct2.status_Led = on_state;


       //Write from parent to child
        write(parent[1], &pipe_struct2, sizeof(pipe_struct2));
        fp = fopen("pipes.txt","a");
        clock_gettime(CLOCK_REALTIME, &t);
        fprintf(fp,"Implementation using pipes\n");
        fprintf(fp,"\nFIRST STRING : %s\n",pipe_struct2.str);
        fprintf(fp,"\nSTRING LENGTH : %d\n",pipe_struct2.str_length);
        fprintf(fp,"\nLED STATE : %d\n",pipe_struct2.status_Led);
        fprintf(fp,"\nSECOND STRING : %s\n",pipe_struct2.str1);
        fprintf(fp,"\nTHIRD STRING : %s\n",pipe_struct2.str2);
        fprintf(fp,"\nFOURTH STRING : %s\n",pipe_struct2.str3);
        fprintf(fp,"\nFIFTH STRING : %s\n",pipe_struct2.str4);
        fprintf(fp,"\nPID-1 : %d\n",pipe_struct2.x);
        fprintf(fp,"\nPID-2 : %d\n",pipe_struct2.x);
        fprintf(fp,"\nPID-3: %d\n",pipe_struct2.x);
        fprintf(fp,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
        fclose(fp);
        wait(NULL);

 

        char my_buffer[sizeof(pipe_struct)];
        read(child[0], my_buffer, sizeof(pipe_struct));
        pipe_led = (pipe_struct*)my_buffer;
      
        fp = fopen("pipes.txt","a");
        clock_gettime(CLOCK_REALTIME, &t);
        fprintf(fp,"\nFIRST STRING : %s\n",pipe_led->str);
        fprintf(fp,"\nSTRING LENGTH : %d\n",pipe_led->str_length);
        fprintf(fp,"\nLED STATE : %d\n",pipe_led->status_Led);
        fprintf(fp,"\nSECOND STRING : %s\n",pipe_led->str1);
        fprintf(fp,"\nTHIRD STRING : %s\n",pipe_led->str2);
        fprintf(fp,"\nFOURTH STRING : %s\n",pipe_led->str3);
        fprintf(fp,"\nFIFTH STRING : %s\n",pipe_led->str4);
        fprintf(fp,"\nPID-1 : %d\n",pipe_led->x);
        fprintf(fp,"\nTID is : %d\n",pipe_led->x);
        fprintf(fp,"\nPTHREAD: %d\n",pipe_led->x);
        fprintf(fp,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
        fclose(fp);


    }
    else
    {
      
        char my_buffer[sizeof(pipe_struct)];
        pipe_struct *pipe_struct2;
        read(parent[0], my_buffer, sizeof(pipe_struct));
        pipe_struct2 = (pipe_struct*)my_buffer;
	  
        fp = fopen("pipes.txt","a");
        fprintf(fp,"Implementation using pipes\n");
        
        clock_gettime(CLOCK_REALTIME, &t);
        fprintf(fp,"\nFIRST STRING : %s\n",pipe_struct2->str);
        fprintf(fp,"\nSTRING LENGTH : %d\n",pipe_struct2->str_length);
        fprintf(fp,"\nLED STATE : %d\n",pipe_struct2->status_Led);
        fprintf(fp,"\nSECOND STRING : %s\n",pipe_struct2->str1);
        fprintf(fp,"\nTHIRD STRING : %s\n",pipe_struct2->str2);
        fprintf(fp,"\nFOURTH STRING : %s\n",pipe_struct2->str3);
        fprintf(fp,"\nFIFTH STRING : %s\n",pipe_struct2->str4);
        fprintf(fp,"\nPID-1 : %d\n",pipe_struct2->x);
        fprintf(fp,"\nPID-2 : %d\n",pipe_struct2->x);
        fprintf(fp,"\nPID-3: %d\n",pipe_struct2->x);
        fprintf(fp,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
        fclose(fp);

        

        pipe_struct pipe_struct3;
        char s[] = "Message from Child";
        
        strcpy(pipe_struct3.str,s1p);
        strcpy(pipe_struct3.str1,s2p);
        strcpy(pipe_struct3.str2,s3p);
        strcpy(pipe_struct3.str3,s4p);
        strcpy(pipe_struct3.str4,s5p);
     

        strcpy(pipe_struct3.str,s);
        pipe_struct3.str_length = strlen(pipe_struct3.str);
        pipe_struct3.status_Led = off_state;

        write(child[1], &pipe_struct3, sizeof(pipe_struct));

        fp = fopen("pipes.txt","a");
        clock_gettime(CLOCK_REALTIME, &t);
        fprintf(fp,"\nNEW FIRST STRING : %s\n",pipe_struct3.str);
        fprintf(fp,"\nSTRING LENGTH : %d\n",pipe_struct3.str_length);
        fprintf(fp,"\nLED STATE : %d\n",pipe_struct3.status_Led);
        fprintf(fp,"\nSECOND STRING : %s\n",pipe_struct3.str1);
        fprintf(fp,"\nTHIRD STRING : %s\n",pipe_struct3.str2);
        fprintf(fp,"\nFOURTH STRING : %s\n",pipe_struct3.str3);
        fprintf(fp,"\nFIFTH STRING : %s\n",pipe_struct3.str4);
        fprintf(fp,"\nPID-1 : %d\n",pipe_struct3.x);
        fprintf(fp,"\nPID-2 : %d\n",pipe_struct3.x);
        fprintf(fp,"\nPID-3: %d\n",pipe_struct3.x);
        fprintf(fp,"\nTIMESTAMP PARENT TO CHILD: %ld \n", t.tv_nsec);
        fclose(fp);
 	      flag = 1;
        exit(0);

        
    }
}
