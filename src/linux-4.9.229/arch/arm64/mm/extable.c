/*
 * Based on arch/arm/mm/extable.c
 */

#include <linux/extable.h>
#include <linux/uaccess.h>

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;
	printk("\n\n\nfixup_exception\n\n\n");
	fixup = search_exception_tables(instruction_pointer(regs));
	if (fixup)
		regs->pc = (unsigned long)&fixup->fixup + fixup->fixup;

	return fixup != NULL;
}
