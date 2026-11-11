// 10_atomic_counter.c - /proc/kcounter: 읽을 때마다 atomic 카운터가 1씩 증가
#include <linux/atomic.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static atomic_t counter = ATOMIC_INIT(0);

static int kcounter_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", atomic_inc_return(&counter));
	return 0;
}

static int kcounter_open(struct inode *inode, struct file *file)
{
	return single_open(file, kcounter_show, NULL);
}

static const struct proc_ops kcounter_ops = {
	.proc_open = kcounter_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init kcounter_init(void)
{
	if (!proc_create("kcounter", 0444, NULL, &kcounter_ops))
		return -ENOMEM;
	pr_info("kcounter: /proc/kcounter created\n");
	return 0;
}

static void __exit kcounter_exit(void)
{
	remove_proc_entry("kcounter", NULL);
	pr_info("kcounter: removed (last=%d)\n", atomic_read(&counter));
}

module_init(kcounter_init);
module_exit(kcounter_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Atomic counter exposed via /proc");
