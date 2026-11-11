// 06_timer.c - 커널 타이머: 2초마다 반복 실행
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/timer.h>

static struct timer_list my_timer;
static unsigned int count;

static void timer_cb(struct timer_list *t)
{
	pr_info("timer: fired %u time(s)\n", ++count);
	mod_timer(&my_timer, jiffies + 2 * HZ);
}

static int __init timer_example_init(void)
{
	timer_setup(&my_timer, timer_cb, 0);
	mod_timer(&my_timer, jiffies + 2 * HZ);
	pr_info("timer: started\n");
	return 0;
}

static void __exit timer_example_exit(void)
{
	timer_shutdown_sync(&my_timer);
	pr_info("timer: stopped after %u fire(s)\n", count);
}

module_init(timer_example_init);
module_exit(timer_example_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Periodic kernel timer example");
