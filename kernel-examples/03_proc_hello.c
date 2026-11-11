// 03_proc_hello.c - /proc/hello_proc 파일 만들기 (cat /proc/hello_proc)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int hello_show(struct seq_file *m, void *v)
{
	seq_puts(m, "hello from /proc\n");
	return 0;
}

static int hello_open(struct inode *inode, struct file *file)
{
	return single_open(file, hello_show, NULL);
}

static const struct proc_ops hello_proc_ops = {
	.proc_open = hello_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init proc_hello_init(void)
{
	if (!proc_create("hello_proc", 0444, NULL, &hello_proc_ops))
		return -ENOMEM;
	pr_info("proc_hello: /proc/hello_proc created\n");
	return 0;
}

static void __exit proc_hello_exit(void)
{
	remove_proc_entry("hello_proc", NULL);
	pr_info("proc_hello: /proc/hello_proc removed\n");
}

module_init(proc_hello_init);
module_exit(proc_hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple /proc file example");
