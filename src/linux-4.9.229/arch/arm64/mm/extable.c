/*
 * Based on arch/arm/mm/extable.c
 */

#include <linux/extable.h>
#include <linux/uaccess.h>

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;
	/**
	 * 根据发生错误的 pc 选择需要跳转的 pc
	 */
	fixup = search_exception_tables(instruction_pointer(regs));
	if (fixup)
		regs->pc = (unsigned long)&fixup->fixup + fixup->fixup;
	printk(KERN_INFO "%s : pc[%lx]\n", __func__, regs->pc);

	return fixup != NULL;
}
