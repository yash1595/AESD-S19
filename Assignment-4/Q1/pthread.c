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

pid_t getpid(void);

void *print_message_function( void *ptr );

typedef struct {
	char* filename;
     int numofthreads;
     char* length;
}FileStruct;



FileStruct fileptr;
FileStruct* file = &fileptr;
FILE* fp;

void *print_thread_specifics(void* thread_message);
void* child_thread_1(void*);
void* child_thread_2(void*);

struct timeval tv;
time_t curtime;
struct timespec time_measure;
struct timeval current_time;
char buffer[30];
char characters[26], i, T_Count;
int count[26];
char T2, T2;
typedef enum{DEFAULT=0,DONE=1,PENDING=2}STATUS;
STATUS flag;
int status_flag=1;

int main(int argc, char **argv)
{
     flag=DEFAULT;
     pthread_t thread1, thread2;
     char *main_message = "Main Thread";
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

     printf("File name entered:");

     file->filename = argv[1];
     strcat(file->filename,".txt");
     printf("%s",file->filename);
     fp = fopen(file->filename,"w+");

     iret1 = pthread_create( &thread1, NULL, child_thread_1, message1);
     
     iret2 = pthread_create( &thread2, NULL, child_thread_2, message2);
    pthread_join( thread2, NULL); 
   
}

void ExitFunction(int status)
{

    if(status == SIGVTALRM)
     {
      printf("PID:%d\n",getpid());
      flag=DONE;
     }

    else if(status == SIGUSR1 || status == SIGUSR2)
      {
        
        if(status_flag==SIGUSR1)status_flag=1;
        else status_flag = 0;
        flag = PENDING;
      }

}


void *child_thread_1(void* thread_message)
{
	fp = fopen(file->filename,"a+");
	fprintf(fp,"%s accessing the file\n" "PID:%d\n" "TID:%ld\n",(char*)thread_message,getpid(),syscall(SYS_gettid));
	 
   
  long int TID = syscall(SYS_gettid);
  long int TID_SELF = pthread_self();
  static FILE *fp1 = 0;
  static FILE *fp2;
  if(fp2<0)
  {
    printf("\nNot open file\n");
  }
  fp2 = fopen(file->filename, "a");
  gettimeofday(&current_time, NULL);
  fprintf(fp2, "\n THREAD-1 STARTED: %lu.%lu \n", current_time.tv_sec, current_time.tv_usec);
  fprintf(fp2, "LINUX-ID : %lu POSIX-ID : %lu",TID,TID_SELF);
  fp1 = fopen("gdb.txt","r");
  int a;
  char ch,c;
  for(ch='A',i=0;ch<'Z',i<26;++ch,++i){characters[i]=ch;}//printf("%c",characters[i]);}
  while(1)
  {
    c=fgetc(fp1);
    if(feof(fp1))break;
    for(i=0;i<26;++i)
    {
      if(characters[i]==(char)(c) || characters[i]+32==(char)(c))++count[i];
    }
  }
  fprintf(fp2,"\n");
  for(i=0;i<26;++i)
  {
    if(count[i]<100)fprintf(fp2,"%c/%c has a count of %d\n",characters[i],characters[i]+32,count[i]);
  }

  gettimeofday(&current_time, NULL);
  fprintf(fp2, "\nThread 1 over at time %lu.%lu and process is completed \n", current_time.tv_sec, current_time.tv_usec);
  fclose(fp2);
  fclose(fp1);
  pthread_exit(0);
  
}

void *child_thread_2(void* thread_message)
{
	
     struct sigaction sig_action_time;
     struct itimerval sig_action_time_timer;
     static FILE *read, *write;

     fp = fopen(file->filename,"a+");
     gettimeofday(&tv, NULL); 
     curtime=tv.tv_sec;
     strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
     fprintf(fp,"%s%ld\t\n",buffer,tv.tv_usec);
	   fprintf(fp,"PID:%d\n TID:%ld\n",getpid(),syscall(SYS_gettid));
     fclose(fp);
     bzero(&sig_action_time, sizeof (sig_action_time));
     sig_action_time.sa_handler = &ExitFunction;
     sig_action_time_timer.it_interval.tv_sec = 0;
     sig_action_time_timer.it_interval.tv_usec = 100000;
     sig_action_time_timer.it_value.tv_sec = 1;
     sig_action_time_timer.it_value.tv_usec = 0;
     sigaction (SIGVTALRM, &sig_action_time, NULL);
     sigaction (SIGUSR1, &sig_action_time, NULL);
     sigaction (SIGUSR2, &sig_action_time, NULL);
     setitimer (ITIMER_VIRTUAL, &sig_action_time_timer, NULL);
         
     char *cpu_cat_process;
     while(1)
     {
      switch(flag)
      {
        case DONE:
                  cpu_cat_process = "cat /proc/stat | head -n 2";
                  read = popen(cpu_cat_process, "r");
                  if(read<0)printf("\n CANNOT OPEN FILE\n");

                  write = fopen(file->filename, "a+");
                  if(write)
                  {
                      gettimeofday(&tv, NULL); 
                      curtime=tv.tv_sec;
                      strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
                      fprintf(write,"Time is: %s%ld\t\n",buffer,tv.tv_sec);

                      while(!feof(read))
                      {
                        char eof_check;
                        eof_check = fgetc(read);
                        fprintf(write, "%c", eof_check);
                      }
            
                  }
                  fclose(write);
                  flag=DEFAULT;
                  break;
        
        

        case DEFAULT: break;

        case PENDING:
                    read = fopen(file->filename, "a");
                    gettimeofday(&tv, NULL);
                    fprintf(read, "\n THREAD-2 TERMINTED %lu.%lu\n", tv.tv_sec, tv.tv_usec);
                    if(status_flag==1)fprintf(read,"-USR1 Terminated\n");
                    else fprintf(read,"-USR2 Terminated\n");
                    fclose(read);
                    exit(-1);
      }

     }
    
     pthread_exit(NULL);
}

