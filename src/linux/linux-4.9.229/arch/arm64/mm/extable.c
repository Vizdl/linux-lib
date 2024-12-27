/*
 * Based on arch/arm/mm/extable.c
 */

#include <linux/extable.h>
#include <linux/uaccess.h>

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;
	unsigned long pc = instruction_pointer(regs);
	/**
	 * 根据发生错误的 pc 选择需要跳转的 pc
	 */
	fixup = search_exception_tables(pc);
	if (fixup)
		regs->pc = (unsigned long)&fixup->fixup + fixup->fixup;
	dl_dbg("old pc[%llx] -> new pc[%llx]", pc, regs->pc);

	return fixup != NULL;
}
