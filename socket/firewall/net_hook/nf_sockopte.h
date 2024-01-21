#ifndef __NF_SOCKOPTE_H__
#define __NF_SOCKOPTE_H__

#define SOE_BANDIP		(0x6001)
#define SOE_BANDPORT	(0x6002)
#define SOE_BANDPING	(0x6003)

// band port sockopt command used struture, copy datum from user space and kernel space
typedef struct nf_bandport
{
	unsigned short protocol;	// band protocol, TCP?UDP 
	unsigned short port;			// band port 
} nf_bandport_t;

typedef struct band_status
{
	unsigned int band_ip;			// the ip user to band, 0 is not band 
	nf_bandport_t band_port;	//port to band, {0,0} is not set 
	unsigned char band_ping;	// whether the ping is band, 0 NO, 1 YES 
} band_status;

#endif

/*
struct nf_hook_ops
{
	struct list_head list;                      　　　　//链表成员
	// User fills in from here down.
	nf_hookfn *hook;                         　　　　　　//钩子函数指针
	struct module *owner;
	int pf;                                            //协议簇, 对于ipv4而言, 是PF_INET
	int hooknum;                                  　　　//hook类型
	// Hooks are ordered in ascending priority.
	int priority;                                      //优先级
};

struct nf_sockopt_ops {
	struct list_head list;
	u_int8_t pf;

	int set_optmin;
	int set_optmax;
	int (*set)(struct sock *sk, int optval, void __user *user, unsigned int len);

	int get_optmin;
	int get_optmax;
	int (*get)(struct sock *sk, int optval, void __user *user, int *len);
};
*/
