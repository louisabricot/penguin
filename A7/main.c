#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/debugfs.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>

/* directory and files */
static struct dentry *dir;
static struct dentry *file;
static char *message = "corozco";
static int message_length = 7;

static ssize_t data_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    return simple_read_from_buffer(buf, count, f_pos, message, strlen(message));
}

static ssize_t data_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
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

const struct file_operations data_file_fops = {
	.owner = THIS_MODULE,
	.read = data_read,
	.write = data_write,
};


static int createDirectory(void)
{
	/* /sys/kernel/debug */
	dir = debugfs_create_dir("fortytwo", NULL);
	if (!dir)
		return -ENOENT;
	pr_info("Directory fortytwo (ok)\n");
	return 0;
}

static int createFile(char *name, int chmod)
{
	file = debugfs_create_file(name, chmod, dir, NULL, &data_file_fops);
	if (!file)
		goto exit;
	pr_info("Directory %s (ok)\n", name);
	return 0;

exit:
	pr_info("Error (CREATEFILE)\n");
	debugfs_remove_recursive(dir);
	return -ENOENT;

}

static int __init debug42_init(void)
{
	pr_info("Menaging envirement . . . . . . . . azereje\n");
	createDirectory();
	createFile("id", 0666);
	createFile("", 0666);
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
