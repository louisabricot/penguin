// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lmalki-h");

#define LOGIN_LEN 8
#define LOGIN "lmalki-h"
#define DEVNAME "fortytwo"

static ssize_t misc_read(struct file *filp,
	char __user *user_buf,
	size_t n,
	loff_t *off)
{
	return simple_read_from_buffer(user_buf, n, off, LOGIN, LOGIN_LEN);
}

static ssize_t misc_write(struct file *filp,
	const char __user *user_buf,
	size_t n,
	loff_t *off)
{
	char buffer[LOGIN_LEN];

	if (n != LOGIN_LEN)
		return -EINVAL;
	if (simple_write_to_buffer(buffer, LOGIN_LEN, off, user_buf, n) < 0)
		return -EINVAL;
	if (strncmp(LOGIN, buffer, LOGIN_LEN) == 0)
		return LOGIN_LEN;
	return -EINVAL;

}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = misc_write,
	.read  = misc_read,
};

static struct miscdevice miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVNAME,
	.fops = &fops,
};

static int misc_start(void)
{
	int error = misc_register(&miscdev);

	if (error) {
		pr_err("Error while registering\n");
		return error;
	}
	pr_info("Loading misc module\n");
	return 0;
}

static void misc_end(void)
{
	misc_deregister(&miscdev);
	pr_info("Unloading misc module\n");
}

module_init(misc_start);
module_exit(misc_end);
