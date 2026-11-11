// 02_param.c - 모듈 파라미터: insmod 02_param.ko name=kernel times=3
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static char *name = "world";
static int times = 1;
module_param(name, charp, 0444);
MODULE_PARM_DESC(name, "who to greet");
module_param(times, int, 0444);
MODULE_PARM_DESC(times, "how many times to greet");

static int __init param_init(void)
{
	int i;

	for (i = 0; i < times; i++)
		pr_info("param: hello, %s (%d/%d)\n", name, i + 1, times);
	return 0;
}

static void __exit param_exit(void)
{
	pr_info("param: bye, %s\n", name);
}

module_init(param_init);
module_exit(param_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module parameter example");
