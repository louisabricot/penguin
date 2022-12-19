// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

/* directory and files */
static struct dentry *dir;
static struct dentry *file;
static char *message = "corozco";
static int message_length = 7;
static char data[PAGE_SIZE];

static ssize_t id_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, message, strlen(message));
}

static ssize_t id_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char specified_msg[count];
	ssize_t retval = -EINVAL;

	if (count != message_length)
		return retval;

	retval = simple_write_to_buffer(specified_msg, count, f_pos, buf, count);
	if (retval < 0)
		return retval;
	retval = strncmp(message, specified_msg, count) ? -EINVAL : count;
	return retval;
}


/* DEFINE_MUTEX defines and initializes mutex. It is useful for define global mutexes. */
DEFINE_MUTEX(mutex);
static ssize_t foo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int retval;

	mutex_lock(&mutex);
	retval = simple_read_from_buffer(buf, count, f_pos, data, strlen(data));
	mutex_unlock(&mutex);
	return retval;
}

static ssize_t foo_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int retval;

	mutex_lock(&mutex);
	retval = simple_write_to_buffer(data, count, f_pos, buf, count);
	mutex_unlock(&mutex);

	return retval;
}

const struct file_operations id_file_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

const struct file_operations foo_file_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static int createDirectory(void)
{
	dir = debugfs_create_dir("fortytwo", NULL);
	if (!dir)
		return -ENOENT;
	pr_info("Directory fortytwo (ok)\n");
	return 0;
}

static int createFile(char *name, int chmod, const struct file_operations *fops)
{
	file = debugfs_create_file(name, chmod, dir, NULL, fops);
	if (!file)
		goto exit;
	pr_info("File %s (ok)\n", name);
	return 0;

exit:
	pr_info("Error [%s] not create\n", name);
	debugfs_remove_recursive(dir);
	return -ENOENT;

}

static int __init debug42_init(void)
{
	pr_info("Menaging envirement . . . . . . . . azereje\n");
	createDirectory();
	createFile("id", 0666, &id_file_fops);
	debugfs_create_ulong("jiffies", 0444, dir, (unsigned long *)&jiffies);
	createFile("foo", 0644, &foo_file_fops);
	return 0;
}

static void __exit debug42_exit(void)
{
	pr_info("Cleaning up module\n");
	debugfs_remove_recursive(dir);
}

module_init(debug42_init);
module_exit(debug42_exit);

MODULE_AUTHOR("corozco");
MODULE_DESCRIPTION("A8");
MODULE_LICENSE("GPL");
