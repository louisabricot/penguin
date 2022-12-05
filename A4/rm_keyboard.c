#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lmalki-h");

static int __init rm_keyboard_init(void) {
 pr_info("USB keyboard removed!\n");
 return 0;
}

static void __exit rm_keyboard_exit(void) {
 pr_info("Cleaning up module\n");
}

module_init(rm_keyboard_init);
module_exit(rm_keyboard_exit);
