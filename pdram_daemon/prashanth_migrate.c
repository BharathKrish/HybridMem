#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/migrate.h>
#include <linux/time64.h>
#include <linux/timekeeping.h>


MODULE_LICENSE("GPL");

static struct timer_list my_timer;
struct timespec64 start_time;
struct timespec64 end_time;
void my_timer_callback(void);


void my_timer_callback(){
    int ret;
    unsigned int nr_pages;
    struct timespec64 time_diff;
     getnstimeofday(&start_time);
     nr_pages = wakeup_migrate();
     //print_alloc_count();
     getnstimeofday(&end_time);
     time_diff=timespec64_sub(end_time,start_time);
     printk("Migrate : nr_pages: %u time_taken: %ld\n",nr_pages,timespec64_to_ns(&time_diff));
     setup_timer(&my_timer, my_timer_callback, 0);
     ret = mod_timer(&my_timer, jiffies+msecs_to_jiffies(5000));
     if(ret) printk("Error in calling the mod_timer again\n");
}

int init_module( void )
{
      int ret;

         /* Queue some additional work (item 2) */
     //Setting up the timer.

     wakeup_migrate();
     setup_timer(&my_timer, my_timer_callback, 0);
     ret = mod_timer(&my_timer, jiffies+msecs_to_jiffies(5000));
     return 0;
}

void cleanup_module( void )
{
      del_timer(&my_timer);
      return;
}
