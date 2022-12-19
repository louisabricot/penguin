// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>

static struct	proc_dir_entry *mymounts;
static char	*buffer;
static int	len;

ssize_t listmounts(struct file *filp, char *user, size_t size, loff_t *off)
{
	struct dentry *curdentry;

	len = 0;
	memset(buffer, 0, PAGE_SIZE);
	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child) {
		if (curdentry->d_flags & DCACHE_MOUNTED) {
			char raw_path[250];

			len += sprintf(buffer + len, "%s\t%s\n", curdentry->d_name.name, dentry_path_raw(curdentry, raw_path, 250));
		}
	}
	return simple_read_from_buffer(user, size, off, buffer, len);
}

static const struct file_operations myops = {
	.owner = THIS_MODULE,
	.read = listmounts,
};

static int __init proc_init(void)
{
	buffer = kmalloc(PAGE_SIZE, GFP_KERNEL);
	mymounts = proc_create("mymounts", 0444, NULL, (const struct proc_ops *)&myops);
	if (!mymounts) {
		pr_err("Error: failed to create mymounts\n");
		return -ENOMEM;
	}
	pr_info("File successfully created!\n");
	return 0;
}

static void __exit proc_exit(void)
{
	kfree(buffer);
	proc_remove(mymounts);
	pr_info("Cleaning up module\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_AUTHOR("corozco");
MODULE_DESCRIPTION("A9");
MODULE_LICENSE("GPL");
