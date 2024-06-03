#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
//#include <linux/sys_config.h>

#define BEEP (13)								//GPIOG13
#define KEY (4)									//GPIOB4
#define LED (6)									//GPIOB6

#define KBUF_SIZE (1024)
#define MIN(x, y) ((x)<(y)?(x):(y))

char wbuf[KBUF_SIZE];						//存储用户传输来的数据
char rbuf[KBUF_SIZE];						//存储用户要读的数据

#define PIO (0X01C20800)				//PIO的基地址
#define PG_BASE ((PIO)+ 0XD8)		//PG的基地址
#define PB_BASE ((PIO)+ 0x24)		//PB的基地址

#define BASE_SIZE 36						//PG组所有寄存器一共的字节大小

unsigned int *vir_pg_base;			//虚拟的PG初始地址
unsigned int *vir_pg_cfg1;			//虚拟的PG第1组寄存器的地址
unsigned int *vir_pg_data;			//虚拟的PG数据寄存器地址

unsigned int *vir_pb_base;			//虚拟的PG初始地址
unsigned int *vir_pb_cfg0;			//虚拟的PG第1组寄存器的地址
unsigned int *vir_pb_data;			//虚拟的PG数据寄存器地址

int gpio_open (struct inode *node, struct file *fp)
{
	//pg13 BEEP
	*vir_pg_cfg1 &= ~(0x7 << 20);
	*vir_pg_cfg1 |= (0x1 << 20);	//配置pg13为输出模式
	*vir_pg_data &= ~(0x1 << 13);	//配置无效电平

	//pb4 KEY
	*vir_pb_cfg0 &= ~(0x7 << 16);	//配置pb4为输入模式

	//pb6 LED
	*vir_pb_cfg0 &= ~(0x7 << 24);
	*vir_pb_cfg0 |= (0x1 << 24);	//配置pb6为输出模式
	*vir_pb_data |= (0x1 << 6);		//配置无效电平，led高电平灭
	return 0;
}

int gpio_release (struct inode *node, struct file *fp)
{
	*vir_pg_data &= ~(0x1 << 13);	//配置无效电平
	*vir_pg_cfg1 |= (0x7 << 20);	//配置pg13为失能 

	*vir_pb_cfg0 |= (0x7 << 16);	//配置pb4为失能

	*vir_pb_data |= (0x1 << 6);		//配置无效电平，led高电平灭
	*vir_pb_cfg0 |= (0x7 << 24);	//配置pb6为失能 

	return 0;
}

ssize_t gpio_read (struct file *fp, char __user * ubuff, size_t size, loff_t * offset)
{
	int ret;
	unsigned char key = (*vir_pb_data >> 4) & 0x01;

	rbuf[0] = key + 48;

	//ret=copy_to_user(ubuff, rbuf, MIN(1, size));
	ret = raw_copy_to_user (ubuff, rbuf, MIN (1, size));
	if (ret != 0)
	{
		pr_err ("copy_to_user error\r\n");
		return -EINVAL;
	}

	return MIN (1, size);
}

ssize_t gpio_write (struct file *fp, const char __user * ubuf, size_t size, loff_t * offset)
{
	int ret;
	memset (wbuf, 0, sizeof (wbuf));
	//ret = copy_from_user(wbuf, ubuf, MIN(KBUF_SIZE, size));
	ret = raw_copy_from_user (wbuf, ubuf, MIN (KBUF_SIZE, size));
	if (ret != 0)
	{
		pr_err ("copy_from_user error\r\n");
		return -EINVAL;
	}

	if (strstr (wbuf, "BEEP_ON"))
		*vir_pg_data |= (0x1 << 13);	//置1
	else if (strstr (wbuf, "BEEP_OFF"))
		*vir_pg_data &= ~(0x1 << 13);	//置0
	else if (strstr (wbuf, "LED_OFF"))
		*vir_pb_data |= (0x1 << 6);
	else if (strstr (wbuf, "LED_ON"))
		*vir_pb_data &= ~(0x1 << 6);

	return MIN (KBUF_SIZE, size);		//返回写入的字节数
}

const struct file_operations gpio_fops = {
	.owner = THIS_MODULE,						//文件操作集的拥有者是本模块
	.read = gpio_read,
	.write = gpio_write,
	.open = gpio_open,
	.release = gpio_release,
};

struct miscdevice mygpio = {
	.minor = MISC_DYNAMIC_MINOR,		//系统自动分配次设备号: my_gpio (与app.c 中的设备名一致, 对齐)
	.name = "my_gpio",
	.fops = &gpio_fops,
};

static int __init gpio_init (void)
{
	int ret;
	ret = misc_register (&mygpio);	//注册一个杂项设备
	if (ret != 0)
	{
		pr_err ("misc_register error");
		return -EINVAL;								//返回错误编码
	}

	//将物理地址映射为虚拟地址
	vir_pg_base = ioremap (PG_BASE, BASE_SIZE);
	if (vir_pg_base == NULL)
	{
		pr_err ("ioremap error");
		goto error1;
	}

	vir_pb_base = ioremap (PB_BASE, BASE_SIZE);
	if (vir_pg_base == NULL)
	{
		pr_err ("ioremap error");
		goto error2;
	}

	vir_pg_cfg1 = vir_pg_base + 1;	//计算出vir_pg_cfg1的地址
	vir_pg_data = vir_pg_base + 4;	//计算出vir_pg_data的地址

	vir_pb_cfg0 = vir_pb_base + 0;	//计算出vir_pb_cfg0的地址
	vir_pb_data = vir_pb_base + 4;	//计算出vir_pb_data的地址

	return 0;

error2:														//error2执行后会继续顺着往下执行，执行error1.
	iounmap (vir_pb_base);					//取消映射

error1:
	misc_deregister (&mygpio);			//从内核注销一个杂项设备
	return -1;
}

static void __exit gpio_exit (void)
{
	misc_deregister (&mygpio);			//从内核注销一个杂项设备
}

module_init (gpio_init);
module_exit (gpio_exit);

MODULE_LICENSE ("GPL");
