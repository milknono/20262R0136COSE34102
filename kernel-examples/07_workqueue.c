// 07_workqueue.c - workqueue: 모듈 로드 시 작업 1개를 지연 실행
#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>

static void work_fn(struct work_struct *w)
{
	pr_info("workqueue: deferred work running in process context\n");
}

static DECLARE_DELAYED_WORK(my_work, work_fn);

static int __init wq_init(void)
{
	schedule_delayed_work(&my_work, msecs_to_jiffies(500));
	pr_info("workqueue: work scheduled\n");
	return 0;
}

static void __exit wq_exit(void)
{
	cancel_delayed_work_sync(&my_work);
	pr_info("workqueue: exit\n");
}

module_init(wq_init);
module_exit(wq_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Delayed work example");
