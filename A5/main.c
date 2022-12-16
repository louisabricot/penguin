// SPDX-License-Identifier: GPL-2.0 OR MIT
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

static char *message = "corozco";
static int message_length = 7;

static ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return simple_read_from_buffer(buf, count, f_pos, message, strlen(message));
}

static ssize_t misc_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
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

const struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.read = misc_read,
	.write = misc_write,
};

static struct miscdevice mini_mics = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &misc_fops,
};

static int misc_init(void)
{
	pr_info("My minor number is: %i\n", mini_mics.minor);
	return misc_register(&mini_mics);
}

static void misc_exit(void)
{
	pr_info("exit\n");
	misc_deregister(&mini_mics);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_AUTHOR("corozco");
MODULE_DESCRIPTION("mini misc driver.");
MODULE_LICENSE("GPL");
