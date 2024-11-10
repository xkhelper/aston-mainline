/* SPDX-License-Identifier: GPL-2.0-or-later */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM pwm

#if !defined(_TRACE_PWM_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_PWM_H

#include <linux/pwm.h>
#include <linux/tracepoint.h>

DECLARE_EVENT_CLASS(pwm,

	TP_PROTO(struct pwm_device *pwm, const struct pwm_state *state, int err),

	TP_ARGS(pwm, state, err),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(struct pwm_device *, pwm)
=======
		__field(unsigned int, chipid)
		__field(unsigned int, hwpwm)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		__field(u64, period)
		__field(u64, duty_cycle)
		__field(enum pwm_polarity, polarity)
		__field(bool, enabled)
		__field(int, err)
	),

	TP_fast_assign(
<<<<<<< HEAD
		__entry->pwm = pwm;
=======
		__entry->chipid = pwm->chip->id;
		__entry->hwpwm = pwm->hwpwm;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		__entry->period = state->period;
		__entry->duty_cycle = state->duty_cycle;
		__entry->polarity = state->polarity;
		__entry->enabled = state->enabled;
		__entry->err = err;
	),

<<<<<<< HEAD
	TP_printk("%p: period=%llu duty_cycle=%llu polarity=%d enabled=%d err=%d",
		  __entry->pwm, __entry->period, __entry->duty_cycle,
=======
	TP_printk("pwmchip%u.%u: period=%llu duty_cycle=%llu polarity=%d enabled=%d err=%d",
		  __entry->chipid, __entry->hwpwm, __entry->period, __entry->duty_cycle,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		  __entry->polarity, __entry->enabled, __entry->err)

);

DEFINE_EVENT(pwm, pwm_apply,

	TP_PROTO(struct pwm_device *pwm, const struct pwm_state *state, int err),

	TP_ARGS(pwm, state, err)
);

DEFINE_EVENT(pwm, pwm_get,

	TP_PROTO(struct pwm_device *pwm, const struct pwm_state *state, int err),

	TP_ARGS(pwm, state, err)
);

#endif /* _TRACE_PWM_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
