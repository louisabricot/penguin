// SPDX-License-Identifier: GPL-2.0 OR MIT
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("corozco");

static int __init usbk_init(void)
{
	pr_info("Usb K connected!\n");
	return 0;
}

static void __exit usbk_exit(void)
{
	pr_info("Usb K deconnected\n");
}

module_init(usbk_init);
module_exit(usbk_exit);
