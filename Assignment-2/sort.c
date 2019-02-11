/*##############################################################################################
@References:
1. https://www.theurbanpenguin.com/using-a-simple-c-program-to-explain-the-suid-permission/
2. https://www.includehelp.com/c-programs/system-date-time-linux.aspx
3. https://www.unixmen.com/add-cron-jobs-linux-unix/
################################################################################################*/

#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <math.h>

#include <stdlib.h>

void uid(void);
void pid(void);
void time1(void);
FILE* fp;
int main(void)
{    
    int i=0,j=0;
    int c, n;
    int input[256]={0};
    printf("256 random numbers\n");
    for (c = 1; c <= 256; c++) {
    n = rand() % 500 + 1;
    input[c-1]=n;
    printf("%d\n",input[c]);
    }
    int size=256;
    int sorted_array[256]={0};
    printf("Unsorted array user space:\n");
    for(i=0;i<256;++i)printf("%d,",input[i]);
    long int status=syscall(399,input,size,sorted_array);
    printf("Sorted array user space:\n");
    for(i=0;i<256;++i)printf("%d,",sorted_array[i]);
    /*******************************************/
    
    fp = fopen("logging.txt","a+");
    uid();
    pid();
    time1();
    for(i=0;i<256;++i)fprintf(fp,"%d,",input[i]);
    for(i=0;i<256;++i)fprintf(fp,"%d,",sorted_array[i]);
    fclose(fp);
}

void uid(void)
{
  int real = getuid();
  int euid = geteuid();
  fprintf(fp,"The REAL UID =: %d/\n", real);
  fprintf(fp,"The EFFECTIVE UID =: %d\n", euid);
   
}

void time1()
{

    time_t T= time(NULL);
    struct  tm tm = *localtime(&T);
     
    fprintf(fp,"System Date is: %02d/%02d/%04d\n",tm.tm_mon+1,tm.tm_mday, 		  tm.tm_year+1900);
    fprintf(fp,"System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
 }

void pid()
{
	//variable to store calling function's process id
	pid_t process_id;
	//variable to store parent function's process id
	pid_t p_process_id;

	//getpid() - will return process id of calling function
	process_id = getpid();
	//getppid() - will return process id of parent function
	p_process_id = getppid();

	//printing the process ids
	fprintf(fp,"The process id: %d\n",process_id);
	fprintf(fp,"The process id of parent function: %d\n",p_process_id);

	return;
}
