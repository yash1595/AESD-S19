#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>

// Reference : https://gist.github.com/itrobotics/596443150c01ff54658e

int diff = 500;
struct timer_list TimerMod;
char *name = "Yashh";
int count;

module_param(name, charp , S_IRUSR | S_IWUSR);
module_param(count, int , S_IRUSR | S_IWUSR);
module_param(diff, int , S_IRUSR | S_IWUSR);

void _timerHandler(unsigned long data)
{
    mod_timer( &TimerMod, jiffies + msecs_to_jiffies(diff));
    ++count;
    printk(KERN_ALERT "%s : Count: %d\n", name,count);
}

static int __init InitTimer(void)
{
    setup_timer(&TimerMod,_timerHandler, 0);
    mod_timer( &TimerMod, jiffies + msecs_to_jiffies(diff));
    return 0;
}

static void __exit ExitTimer(void)
{
    del_timer(&TimerMod);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yash Gupte");
MODULE_DESCRIPTION("MY TIMER MODULE");

module_init(InitTimer);
module_exit(ExitTimer);
