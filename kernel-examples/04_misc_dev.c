// 04_misc_dev.c - misc character device: /dev/kecho (write 한 내용을 read 로 돌려줌)
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define BUF_SZ 256

static char buf[BUF_SZ];
static size_t buf_len;
static DEFINE_MUTEX(buf_lock);

static ssize_t kecho_read(struct file *f, char __user *ubuf, size_t len, loff_t *off)
{
	ssize_t ret;

	mutex_lock(&buf_lock);
	ret = simple_read_from_buffer(ubuf, len, off, buf, buf_len);
	mutex_unlock(&buf_lock);
	return ret;
}

static ssize_t kecho_write(struct file *f, const char __user *ubuf, size_t len, loff_t *off)
{
	size_t n = min_t(size_t, len, BUF_SZ);

	mutex_lock(&buf_lock);
	if (copy_from_user(buf, ubuf, n)) {
		mutex_unlock(&buf_lock);
		return -EFAULT;
	}
	buf_len = n;
	mutex_unlock(&buf_lock);
	return n;
}

static const struct file_operations kecho_fops = {
	.owner = THIS_MODULE,
	.read = kecho_read,
	.write = kecho_write,
};

static struct miscdevice kecho_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "kecho",
	.fops = &kecho_fops,
	.mode = 0666,
};

static int __init kecho_init(void)
{
	int ret = misc_register(&kecho_dev);

	if (!ret)
		pr_info("kecho: /dev/kecho registered\n");
	return ret;
}

static void __exit kecho_exit(void)
{
	misc_deregister(&kecho_dev);
	pr_info("kecho: unregistered\n");
}

module_init(kecho_init);
module_exit(kecho_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Echo misc device example");
