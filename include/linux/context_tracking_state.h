/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CONTEXT_TRACKING_STATE_H
#define _LINUX_CONTEXT_TRACKING_STATE_H

#include <linux/percpu.h>
#include <linux/static_key.h>
#include <linux/context_tracking_irq.h>

/* Offset to allow distinguishing irq vs. task-based idle entry/exit. */
<<<<<<< HEAD
#define DYNTICK_IRQ_NONIDLE	((LONG_MAX / 2) + 1)

enum ctx_state {
	CONTEXT_DISABLED	= -1,	/* returned by ct_state() if unknown */
	CONTEXT_KERNEL		= 0,
	CONTEXT_IDLE		= 1,
	CONTEXT_USER		= 2,
	CONTEXT_GUEST		= 3,
	CONTEXT_MAX		= 4,
};

/* Even value for idle, else odd. */
#define RCU_DYNTICKS_IDX CONTEXT_MAX

#define CT_STATE_MASK (CONTEXT_MAX - 1)
#define CT_DYNTICKS_MASK (~CT_STATE_MASK)
=======
#define CT_NESTING_IRQ_NONIDLE	((LONG_MAX / 2) + 1)

enum ctx_state {
	CT_STATE_DISABLED	= -1,	/* returned by ct_state() if unknown */
	CT_STATE_KERNEL		= 0,
	CT_STATE_IDLE		= 1,
	CT_STATE_USER		= 2,
	CT_STATE_GUEST		= 3,
	CT_STATE_MAX		= 4,
};

/* Odd value for watching, else even. */
#define CT_RCU_WATCHING CT_STATE_MAX

#define CT_STATE_MASK (CT_STATE_MAX - 1)
#define CT_RCU_WATCHING_MASK (~CT_STATE_MASK)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

struct context_tracking {
#ifdef CONFIG_CONTEXT_TRACKING_USER
	/*
	 * When active is false, probes are unset in order
	 * to minimize overhead: TIF flags are cleared
	 * and calls to user_enter/exit are ignored. This
	 * may be further optimized using static keys.
	 */
	bool active;
	int recursion;
#endif
#ifdef CONFIG_CONTEXT_TRACKING
	atomic_t state;
#endif
#ifdef CONFIG_CONTEXT_TRACKING_IDLE
<<<<<<< HEAD
	long dynticks_nesting;		/* Track process nesting level. */
	long dynticks_nmi_nesting;	/* Track irq/NMI nesting level. */
=======
	long nesting;		/* Track process nesting level. */
	long nmi_nesting;	/* Track irq/NMI nesting level. */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif
};

#ifdef CONFIG_CONTEXT_TRACKING
DECLARE_PER_CPU(struct context_tracking, context_tracking);
#endif

#ifdef CONFIG_CONTEXT_TRACKING_USER
static __always_inline int __ct_state(void)
{
	return raw_atomic_read(this_cpu_ptr(&context_tracking.state)) & CT_STATE_MASK;
}
#endif

#ifdef CONFIG_CONTEXT_TRACKING_IDLE
<<<<<<< HEAD
static __always_inline int ct_dynticks(void)
{
	return atomic_read(this_cpu_ptr(&context_tracking.state)) & CT_DYNTICKS_MASK;
}

static __always_inline int ct_dynticks_cpu(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return atomic_read(&ct->state) & CT_DYNTICKS_MASK;
}

static __always_inline int ct_dynticks_cpu_acquire(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return atomic_read_acquire(&ct->state) & CT_DYNTICKS_MASK;
}

static __always_inline long ct_dynticks_nesting(void)
{
	return __this_cpu_read(context_tracking.dynticks_nesting);
}

static __always_inline long ct_dynticks_nesting_cpu(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return ct->dynticks_nesting;
}

static __always_inline long ct_dynticks_nmi_nesting(void)
{
	return __this_cpu_read(context_tracking.dynticks_nmi_nesting);
}

static __always_inline long ct_dynticks_nmi_nesting_cpu(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return ct->dynticks_nmi_nesting;
=======
static __always_inline int ct_rcu_watching(void)
{
	return atomic_read(this_cpu_ptr(&context_tracking.state)) & CT_RCU_WATCHING_MASK;
}

static __always_inline int ct_rcu_watching_cpu(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return atomic_read(&ct->state) & CT_RCU_WATCHING_MASK;
}

static __always_inline int ct_rcu_watching_cpu_acquire(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return atomic_read_acquire(&ct->state) & CT_RCU_WATCHING_MASK;
}

static __always_inline long ct_nesting(void)
{
	return __this_cpu_read(context_tracking.nesting);
}

static __always_inline long ct_nesting_cpu(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return ct->nesting;
}

static __always_inline long ct_nmi_nesting(void)
{
	return __this_cpu_read(context_tracking.nmi_nesting);
}

static __always_inline long ct_nmi_nesting_cpu(int cpu)
{
	struct context_tracking *ct = per_cpu_ptr(&context_tracking, cpu);

	return ct->nmi_nesting;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
#endif /* #ifdef CONFIG_CONTEXT_TRACKING_IDLE */

#ifdef CONFIG_CONTEXT_TRACKING_USER
extern struct static_key_false context_tracking_key;

static __always_inline bool context_tracking_enabled(void)
{
	return static_branch_unlikely(&context_tracking_key);
}

static __always_inline bool context_tracking_enabled_cpu(int cpu)
{
	return context_tracking_enabled() && per_cpu(context_tracking.active, cpu);
}

<<<<<<< HEAD
static inline bool context_tracking_enabled_this_cpu(void)
=======
static __always_inline bool context_tracking_enabled_this_cpu(void)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	return context_tracking_enabled() && __this_cpu_read(context_tracking.active);
}

/**
 * ct_state() - return the current context tracking state if known
 *
 * Returns the current cpu's context tracking state if context tracking
 * is enabled.  If context tracking is disabled, returns
<<<<<<< HEAD
 * CONTEXT_DISABLED.  This should be used primarily for debugging.
=======
 * CT_STATE_DISABLED.  This should be used primarily for debugging.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 */
static __always_inline int ct_state(void)
{
	int ret;

	if (!context_tracking_enabled())
<<<<<<< HEAD
		return CONTEXT_DISABLED;
=======
		return CT_STATE_DISABLED;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	preempt_disable();
	ret = __ct_state();
	preempt_enable();

	return ret;
}

#else
static __always_inline bool context_tracking_enabled(void) { return false; }
static __always_inline bool context_tracking_enabled_cpu(int cpu) { return false; }
static __always_inline bool context_tracking_enabled_this_cpu(void) { return false; }
#endif /* CONFIG_CONTEXT_TRACKING_USER */

#endif
