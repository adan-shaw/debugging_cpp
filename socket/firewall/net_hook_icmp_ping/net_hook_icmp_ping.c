#include <linux/time.h>
#include <linux/netdevice.h>
#include "linux/module.h"
#include "linux/netfilter_ipv4.h"
#include "linux/kernel.h"
#include "linux/kern_levels.h"
#include "linux/skbuff.h"
#include "linux/ip.h"
#include <linux/inet.h>
#include "linux/if_ether.h"
#include "linux/if_packet.h"
#include "linux/init.h"

static struct timespec start;
static struct timespec now;

//copyright statement
MODULE_LICENSE ("Dual BSD/GPL");

static unsigned int nf_hook_out (unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn) (struct sk_buff *))
{
	struct sk_buff *sk = skb;
	struct iphdr *iph = ip_hdr (sk);

	now = current_kernel_time ();

	if (iph->protocol == IPPROTO_ICMP)
	{
		if (now.tv_sec - start.tv_sec < 600)
		{
			return NF_DROP;
		}
		else
		{
			return NF_ACCEPT;
		}
	}
	else
	{
		return NF_ACCEPT;
	}
}

static struct nf_hook_ops nfout = {
	.hook = (void*)nf_hook_out,
	.hooknum = NF_INET_LOCAL_OUT,
	.pf = PF_INET,
	.priority = NF_IP_PRI_FIRST
};

static struct net m_net;//未初始化(但貌似不需要初始化, 这个只是tmp 中间变量?)

//initialization module
static int __init banping_init (void)
{
	//linux 4.3以下
	//nf_register_hook (&nfout);
	//linux 4.13及以上
	nf_register_net_hook (&m_net, &nfout);
	start = current_kernel_time ();
	printk (KERN_ALERT "net_hook_icmp_ping module init%ld\n", start.tv_sec);
	return 0;
}

//clear module
static void __exit banping_exit (void)
{
	//linux 4.3以下
	//nf_unregister_hook (&nfout);
	//linux 4.13及以上
	nf_unregister_net_hook (&m_net, &nfout);
	printk (KERN_ALERT "net_hook_icmp_ping module exit\n");
}

module_init (banping_init);
module_exit (banping_exit);

MODULE_AUTHOR ("adan_shaw@qq.com");
MODULE_DESCRIPTION ("net_hook_icmp_ping");
MODULE_VERSION ("0.0.1");
