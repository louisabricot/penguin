// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/jiffies.h>
#include <linux/const.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lmalki-h");

#define LOGIN_LEN 8
#define LOGIN "lmalki-h"

struct	dentry *dir;
struct	dentry *id;
struct	dentry *foo;
int	data_len;
char	data[PAGE_SIZE];
char	login[LOGIN_LEN];
struct	mutex foo_mutex;


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
	if (n != LOGIN_LEN ||
	simple_write_to_buffer(login, LOGIN_LEN, off, user_buf, n) < 0)
		return -EINVAL;
	if (strncmp(LOGIN, login, LOGIN_LEN) == 0)
		return LOGIN_LEN;
	return -EINVAL;

}

static const struct file_operations idfops = {
	.owner = THIS_MODULE,
	.read = misc_read,
	.write = misc_write,
};

static ssize_t foo_write(struct file *filp,
		const char __user *user_buf,
		size_t n,
		loff_t *off)
{
	int ret;

	mutex_lock(&foo_mutex);
	pr_info("Write mutex locked\n");
	if (n > PAGE_SIZE - 1)
		n = PAGE_SIZE - 1;
	memset(&data, 0, PAGE_SIZE);
	ret = simple_write_to_buffer(data, PAGE_SIZE, off, user_buf, n);
	if (ret > 0)
		data_len = ret;
	mutex_unlock(&foo_mutex);
	pr_info("Write mutex unlocked\n");
	return ret;
}

static ssize_t foo_read(struct file *filp,
		char __user *user_buf,
		size_t n,
		loff_t *off)
{
	int ret;

	mutex_lock(&foo_mutex);
	pr_info("Read mutex locked\n");
	ret = simple_read_from_buffer(user_buf, n, off, data, data_len);
	mutex_unlock(&foo_mutex);
	pr_info("Read mutex unlocked\n");
	return ret;
}

static const struct file_operations foofops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static void debugfs_end(void)
{
	debugfs_remove_recursive(dir);
	pr_info("Unloading debugfs module\n");
}

static int debugfs_start(void)
{
	umode_t id_mode;

	id_mode = 0666;
	pr_info("Loading debugfs module\n");
	mutex_init(&foo_mutex);
	data_len = 0;
	dir = debugfs_create_dir("fortytwo", NULL);
	id = debugfs_create_file("id", id_mode, dir, NULL, &idfops);
	foo = debugfs_create_file("foo", 0644, dir, NULL, &foofops);
	debugfs_create_ulong("jiffies", 0444, dir, (unsigned long *)&jiffies);
	return 0;
}

module_init(debugfs_start);
module_exit(debugfs_end);
