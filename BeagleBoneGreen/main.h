#include <stdio.h>
#include <math.h>
#include <float.h>
#include <complex.h>
#include <time.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <malloc.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

void read_temp(void);
void read_distance(void);
void read_smoke(void);


typedef enum{gpio66=0,gpio67=1,gpio68=2,gpio69=3,gpio44=4,gpio117=5,gpio49=6,gpio27=7}PINS;

char* LED_Dir_Path[]={"/sys/class/gpio/gpio66/direction",
					  "/sys/class/gpio/gpio67/direction",
					  "/sys/class/gpio/gpio68/direction",
					  "/sys/class/gpio/gpio69/direction",
            "/sys/class/gpio/gpio44/direction",
            "/sys/class/gpio/gpio117/direction",
          "/sys/class/gpio/gpio49/direction",
          "/sys/class/gpio/gpio27/direction"};

char* LED_Val_Path[]={"/sys/class/gpio/gpio66/value",
					  "/sys/class/gpio/gpio67/value",
					  "/sys/class/gpio/gpio68/value",
					  "/sys/class/gpio/gpio69/value",
          "/sys/class/gpio/gpio44/value",
        "/sys/class/gpio/gpio117/value",
      "/sys/class/gpio/gpio49/value",
    "/sys/class/gpio/gpio27/value"};
