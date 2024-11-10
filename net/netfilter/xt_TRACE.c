// SPDX-License-Identifier: GPL-2.0-only
/* This is a module which is used to mark packets for tracing.
 */
#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/x_tables.h>
#include <net/netfilter/nf_log.h>

MODULE_DESCRIPTION("Xtables: packet flow tracing");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_TRACE");
MODULE_ALIAS("ip6t_TRACE");

static int trace_tg_check(const struct xt_tgchk_param *par)
{
	return nf_logger_find_get(par->family, NF_LOG_TYPE_LOG);
}

static void trace_tg_destroy(const struct xt_tgdtor_param *par)
{
	nf_logger_put(par->family, NF_LOG_TYPE_LOG);
}

static unsigned int
trace_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	skb->nf_trace = 1;
	return XT_CONTINUE;
}

<<<<<<< HEAD
static struct xt_target trace_tg_reg __read_mostly = {
	.name		= "TRACE",
	.revision	= 0,
	.family		= NFPROTO_UNSPEC,
	.table		= "raw",
	.target		= trace_tg,
	.checkentry	= trace_tg_check,
	.destroy	= trace_tg_destroy,
	.me		= THIS_MODULE,
=======
static struct xt_target trace_tg_reg[] __read_mostly = {
	{
		.name		= "TRACE",
		.revision	= 0,
		.family		= NFPROTO_IPV4,
		.table		= "raw",
		.target		= trace_tg,
		.checkentry	= trace_tg_check,
		.destroy	= trace_tg_destroy,
		.me		= THIS_MODULE,
	},
#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
	{
		.name		= "TRACE",
		.revision	= 0,
		.family		= NFPROTO_IPV6,
		.table		= "raw",
		.target		= trace_tg,
		.checkentry	= trace_tg_check,
		.destroy	= trace_tg_destroy,
		.me		= THIS_MODULE,
	},
#endif
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
};

static int __init trace_tg_init(void)
{
<<<<<<< HEAD
	return xt_register_target(&trace_tg_reg);
=======
	return xt_register_targets(trace_tg_reg, ARRAY_SIZE(trace_tg_reg));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static void __exit trace_tg_exit(void)
{
<<<<<<< HEAD
	xt_unregister_target(&trace_tg_reg);
=======
	xt_unregister_targets(trace_tg_reg, ARRAY_SIZE(trace_tg_reg));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

module_init(trace_tg_init);
module_exit(trace_tg_exit);
MODULE_SOFTDEP("pre: nf_log_syslog");
