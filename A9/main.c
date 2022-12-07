#include <linux/kernel.h>
#include <linux/proc_f.h>
#include <linux/module.h>

MODULE_AUTHOR("lmalki-h");
MODULE_LICENSE("GPL");

#define procfs_name "mymounts"

int  init_module()
{
	proc_file = create_proc_entry(procfs_name, 0644, NULL);

	if (proc_file == NULL) {
		remove_proc_entry(procfs_name, &proc_root);
		pr_info("Error: could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}

	proc_file->read_proc = procfile_read;
	proc_file->owner = THIS_MODULE;
	proc_file->mode = S_IFREG | S_IRUGO;
	proc_file->uid = 0;
	proc_file->gid = 0;
	proc_file->size = 37;

	pr_info("/proc/%s created\n");
	return 0;
}
void cleanup_module()
{
	remove_mymounts_entry(procfs_name, &proc_root);
	pr_info("/proc/%s removed", procfs_name);
}
module_init(proc_init);
module_exit(proc_exit);

