#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/migrate.h>
#include <linux/time64.h>
#include <linux/timekeeping.h>


MODULE_LICENSE("GPL");

static struct workqueue_struct *my_wq;
static struct timer_list my_timer;
typedef struct {
      struct work_struct my_work;
        int    x;
} my_work_t;
struct timespec64 start_time;
struct timespec64 end_time;
my_work_t *work, *work2;
void my_timer_callback(void);

static void my_wq_function( struct work_struct *work)
{
      my_work_t *my_work = (my_work_t *)work;
      printk( "my_work.x %d\n", my_work->x );
      kfree( (void *)work );
      return;
}

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
     ret = mod_timer(&my_timer, jiffies+msecs_to_jiffies(300));
     if(ret) printk("Error in calling the mod_timer again\n");
}

int init_module( void )
{
      int ret;
      my_wq = create_workqueue("my_queue");
      if (my_wq) {
         /* Queue some work (item 1) */
         work = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
         if (work) {
            INIT_WORK( (struct work_struct *)work, my_wq_function );
            work->x = 1;
            ret = queue_work( my_wq, (struct work_struct *)work );

         }

         /* Queue some additional work (item 2) */
         work2 = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
         if (work2) {

            INIT_WORK( (struct work_struct *)work2, my_wq_function );

            work2->x = 2;

            ret = queue_work( my_wq, (struct work_struct *)work2 );
         }
     }

     //Setting up the timer.

     setup_timer(&my_timer, my_timer_callback, 0);
     ret = mod_timer(&my_timer, jiffies+msecs_to_jiffies(10));
     wakeup_migrate();
     return 0;
}

void cleanup_module( void )
{
      flush_workqueue( my_wq );
      destroy_workqueue( my_wq );
      del_timer(&my_timer);
      return;
}
