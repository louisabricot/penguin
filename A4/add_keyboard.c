// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lmalki-h");

static int __init add_keyboard_init(void)
{
	pr_info("USB keyboard connected! YOUHOU\n");
	return 0;
}

static void __exit add_keyboard_exit(void)
{
	pr_info("Cleaning up module\n");
}

module_init(add_keyboard_init);
module_exit(add_keyboard_exit);
