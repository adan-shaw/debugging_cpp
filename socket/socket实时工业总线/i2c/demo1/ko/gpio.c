#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
//#include <linux/sys_config.h>

//#define copy_to_userEx copy_to_user       //拷贝数据 to 用户空间
#define copy_to_userEx raw_copy_to_user     //拷贝数据 to 用户空间(x86 专用)

//#define copy_from_userEx copy_from_user   //拷贝数据 from 用户空间
#define copy_from_userEx raw_copy_from_user //拷贝数据 from 用户空间(x86 专用)

#define BEEP (13)										//GPIO G13
#define KEY (4)											//GPIO B4
#define LED (6)											//GPIO B6

#define KBUF_SIZE (1024)
#define MIN(x, y) ((x)<(y)?(x):(y))

char g_wbuf[KBUF_SIZE];							//存储用户传输来的数据
char g_rbuf[KBUF_SIZE];							//存储用户要读的数据

#define PIO (0X01C20800)						//PIO的基地址
#define PG_BASE ((PIO)+ 0XD8)				//PG的基地址
#define PB_BASE ((PIO)+ 0x24)				//PB的基地址

#define BASE_SIZE (36)							//PG组所有寄存器一共的字节大小

unsigned int *g_virt_pg_base;				//虚拟的PG初始地址
unsigned int *g_virt_pg_cfg1;				//虚拟的PG第1组寄存器的地址
unsigned int *g_virt_pg_data;				//虚拟的PG数据寄存器地址

unsigned int *g_virt_pb_base;				//虚拟的PG初始地址
unsigned int *g_virt_pb_cfg0;				//虚拟的PG第1组寄存器的地址
unsigned int *g_virt_pb_data;				//虚拟的PG数据寄存器地址

int gpio_open (struct inode *node, struct file *fp)
{
	//pg13 BEEP
	*g_virt_pg_cfg1 &= ~(0x7 << 20);
	*g_virt_pg_cfg1 |= (0x1 << 20);		//配置pg13为输出模式
	*g_virt_pg_data &= ~(0x1 << 13);	//配置无效电平

	//pb4 KEY
	*g_virt_pb_cfg0 &= ~(0x7 << 16);	//配置pb4为输入模式

	//pb6 LED
	*g_virt_pb_cfg0 &= ~(0x7 << 24);
	*g_virt_pb_cfg0 |= (0x1 << 24);		//配置pb6为输出模式
	*g_virt_pb_data |= (0x1 << 6);		//配置无效电平, led高电平灭
	return 0;
}

int gpio_release (struct inode *node, struct file *fp)
{
	*g_virt_pg_data &= ~(0x1 << 13);	//配置无效电平
	*g_virt_pg_cfg1 |= (0x7 << 20);		//配置pg13为失能 

	*g_virt_pb_cfg0 |= (0x7 << 16);		//配置pb4为失能

	*g_virt_pb_data |= (0x1 << 6);		//配置无效电平, led高电平灭
	*g_virt_pb_cfg0 |= (0x7 << 24);		//配置pb6为失能 
	return 0;
}

ssize_t gpio_read (struct file *fp, char __user * ubuff, size_t size, loff_t * offset)
{
	unsigned char key = (*g_virt_pb_data >> 4) & 0x01;

	g_rbuf[0] = key + 48;

	if (copy_to_userEx (ubuff, g_rbuf, MIN (1, size)) != 0)
	{
		pr_err ("copy_to_userEx() error\r\n");
		return -EINVAL;
	}
	else
		return MIN (1, size);
}

ssize_t gpio_write (struct file *fp, const char __user * ubuf, size_t size, loff_t * offset)
{
	memset (g_wbuf, 0, sizeof (g_wbuf));
	if (copy_to_userEx (g_wbuf, ubuf, MIN (KBUF_SIZE, size)) != 0)
	{
		pr_err ("copy_to_userEx() error\r\n");
		return -EINVAL;
	}

	if (strstr (g_wbuf, "BEEP_ON"))
		*g_virt_pg_data |= (0x1 << 13);	//置1
	else if (strstr (g_wbuf, "BEEP_OFF"))
		*g_virt_pg_data &= ~(0x1 << 13);//置0
	else if (strstr (g_wbuf, "LED_OFF"))
		*g_virt_pb_data |= (0x1 << 6);
	else if (strstr (g_wbuf, "LED_ON"))
		*g_virt_pb_data &= ~(0x1 << 6);

	return MIN (KBUF_SIZE, size);			//返回写入的字节数
}

const struct file_operations gpio_fops = {
	.owner = THIS_MODULE,							//文件操作集的拥有者是本模块
	.read = gpio_read,
	.write = gpio_write,
	.open = gpio_open,
	.release = gpio_release,
};

struct miscdevice mygpio = {
	.minor = MISC_DYNAMIC_MINOR,			//系统自动分配次设备号: my_gpio (与app.c 中的设备名一致, 对齐)
	.name = "my_gpio",
	.fops = &gpio_fops,
};

static int __init gpio_init (void)
{
	if (misc_register (&mygpio) != 0)	//注册一个杂项设备
	{
		pr_err ("misc_register() error");
		return -EINVAL;									//返回错误编码
	}

	//将物理地址映射为虚拟地址
	g_virt_pg_base = ioremap (PG_BASE, BASE_SIZE);
	if (g_virt_pg_base == NULL)
	{
		pr_err ("ioremap() error");
		misc_deregister (&mygpio);				//从内核注销一个杂项设备
		return -1;
	}

	g_virt_pb_base = ioremap (PB_BASE, BASE_SIZE);
	if (g_virt_pb_base == NULL)
	{
		pr_err ("ioremap() error");
		iounmap (g_virt_pg_base);					//取消映射
		misc_deregister (&mygpio);
		return -1;
	}

	g_virt_pg_cfg1 = g_virt_pg_base + 1;//计算出g_virt_pg_cfg1的地址
	g_virt_pg_data = g_virt_pg_base + 4;//计算出g_virt_pg_data的地址

	g_virt_pb_cfg0 = g_virt_pb_base + 0;//计算出g_virt_pb_cfg0的地址
	g_virt_pb_data = g_virt_pb_base + 4;//计算出g_virt_pb_data的地址

	return 0;
}

static void __exit gpio_exit (void)
{
	misc_deregister (&mygpio);				//从内核注销一个杂项设备
}

module_init (gpio_init);						//装载ko 模块的init 初始化函数(对标insmod 命令)
module_exit (gpio_exit);						//装载ko 模块的exit 退出函数(对标rmmod 命令)

MODULE_LICENSE ("GPL");



