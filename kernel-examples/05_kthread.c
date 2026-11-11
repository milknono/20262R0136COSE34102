// 05_kthread.c - 커널 스레드: 1초마다 메시지를 출력하고 모듈 제거 시 종료
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/module.h>

static struct task_struct *worker;

static int worker_fn(void *data)
{
	unsigned int n = 0;

	while (!kthread_should_stop()) {
		pr_info("kthread: tick %u\n", n++);
		msleep_interruptible(1000);
	}
	return 0;
}

static int __init kthread_example_init(void)
{
	worker = kthread_run(worker_fn, NULL, "kexample_thread");
	if (IS_ERR(worker))
		return PTR_ERR(worker);
	pr_info("kthread: started\n");
	return 0;
}

static void __exit kthread_example_exit(void)
{
	kthread_stop(worker);
	pr_info("kthread: stopped\n");
}

module_init(kthread_example_init);
module_exit(kthread_example_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple kernel thread example");
