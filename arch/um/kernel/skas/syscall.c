// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 */

#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/seccomp.h>
#include <kern_util.h>
#include <sysdep/ptrace.h>
#include <sysdep/ptrace_user.h>
#include <sysdep/syscalls.h>
#include <linux/time-internal.h>
#include <asm/unistd.h>
<<<<<<< HEAD
=======
#include <asm/delay.h>
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

void handle_syscall(struct uml_pt_regs *r)
{
	struct pt_regs *regs = container_of(r, struct pt_regs, regs);
	int syscall;

<<<<<<< HEAD
	/*
	 * If we have infinite CPU resources, then make every syscall also a
	 * preemption point, since we don't have any other preemption in this
	 * case, and kernel threads would basically never run until userspace
	 * went to sleep, even if said userspace interacts with the kernel in
	 * various ways.
	 */
	if (time_travel_mode == TT_MODE_INFCPU ||
	    time_travel_mode == TT_MODE_EXTERNAL)
		schedule();

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	/* Initialize the syscall number and default return value. */
	UPT_SYSCALL_NR(r) = PT_SYSCALL_NR(r->gp);
	PT_REGS_SET_SYSCALL_RETURN(regs, -ENOSYS);

	if (syscall_trace_enter(regs))
		goto out;

	/* Do the seccomp check after ptrace; failures should be fast. */
	if (secure_computing() == -1)
		goto out;

	syscall = UPT_SYSCALL_NR(r);
<<<<<<< HEAD
	if (syscall >= 0 && syscall < __NR_syscalls)
		PT_REGS_SET_SYSCALL_RETURN(regs,
				EXECUTE_SYSCALL(syscall, regs));
=======
	if (syscall >= 0 && syscall < __NR_syscalls) {
		unsigned long ret = EXECUTE_SYSCALL(syscall, regs);

		PT_REGS_SET_SYSCALL_RETURN(regs, ret);

		/*
		 * An error value here can be some form of -ERESTARTSYS
		 * and then we'd just loop. Make any error syscalls take
		 * some time, so that it won't just loop if something is
		 * not ready, and hopefully other things will make some
		 * progress.
		 */
		if (IS_ERR_VALUE(ret) &&
		    (time_travel_mode == TT_MODE_INFCPU ||
		     time_travel_mode == TT_MODE_EXTERNAL)) {
			um_udelay(1);
			schedule();
		}
	}
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

out:
	syscall_trace_leave(regs);
}
