// 09_uptime.c - jiffies/HZ/ktime 으로 부팅 후 경과 시간 출력
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>
#include <linux/module.h>

static int __init uptime_init(void)
{
	u64 ms = ktime_to_ms(ktime_get_boottime());

	pr_info("uptime: HZ=%d jiffies=%lu\n", HZ, jiffies);
	pr_info("uptime: boot time %llu ms (%llu s)\n", ms, ms / 1000);
	return 0;
}

static void __exit uptime_exit(void)
{
	pr_info("uptime: bye\n");
}

module_init(uptime_init);
module_exit(uptime_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Print boot time using ktime");
