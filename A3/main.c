// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

void do_work(int *n)
{
	int i;

	for (i = 0; i < *n; ++i)
		udelay(10);

	if (*n < 10)
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!");
}

int my_init(void)
{
	int n = 10;

	do_work(&n);
	return 0;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
