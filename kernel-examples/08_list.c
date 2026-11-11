// 08_list.c - 커널 연결 리스트(list_head) 사용 예제
#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

struct item {
	int value;
	struct list_head node;
};

static LIST_HEAD(items);

static int __init list_example_init(void)
{
	struct item *it;
	int i;

	for (i = 1; i <= 5; i++) {
		it = kmalloc(sizeof(*it), GFP_KERNEL);
		if (!it)
			return -ENOMEM; /* 이미 만든 노드는 exit 이 아닌 여기서 정리해야 함 */
		it->value = i * 10;
		list_add_tail(&it->node, &items);
	}

	list_for_each_entry(it, &items, node)
		pr_info("list: value=%d\n", it->value);
	return 0;
}

static void __exit list_example_exit(void)
{
	struct item *it, *tmp;

	list_for_each_entry_safe(it, tmp, &items, node) {
		list_del(&it->node);
		kfree(it);
	}
	pr_info("list: all nodes freed\n");
}

module_init(list_example_init);
module_exit(list_example_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("list_head example");
