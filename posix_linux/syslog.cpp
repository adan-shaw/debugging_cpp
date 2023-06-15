//编译:
//	g++ ./syslog.cpp -g3 -o x



/*
0.rsyslog service 生成的日志, 主要存放在路径: /var/log
	常见的linux syslog 日志:
		/var/log/message  系统启动后的信息和错误日志, 是Red Hat Linux中最常用的日志之一 
		/var/log/secure   与安全相关的日志信息 
		/var/log/maillog  与邮件相关的日志信息 
		/var/log/cron     与定时任务相关的日志信息 
		/var/log/spooler  与UUCP和news设备相关的日志信息 
		/var/log/boot.log 守护进程启动和停止相关的日志消息 
	(详情请查看: Debian常见syslog日志(改名).html)


1.编程方面:
	syslog 是linux 下的一套日志标准协议, 易用, 简单, 无须编译选项, 直接包含头文件即用.
		#include <syslog.h>
	且posix同样支持syslog, 这意味着: gcc,clang,等unix 编译器, 都支持syslog日志标准.



2.系统配置方面:
	支持syslog 协议的service 软件有很多, 如: rsyslog, syslog-ng, syslogd 等.
	其中, 一般使用rsyslog 比较简单方便.

	rsyslog 支持local 日志模式和network 日志模式,
	但不支持'数据库日志模式', 
	如果需要将日志打入数据库, 请使用: syslog-ng, 详情请看: ../syslog_srv服务器程序安装使用



3.配置好rsyslog 服务, 并启动rsyslog 服务之后,
	就可以直接调用syslog api 直接对日志文件进行简单读写了.

	使用rsyslog 服务, 需要:
		首先需要在rsyslog.conf 配置文件中, 声明你要为哪个程序, 创建syslog 标准日志.
		然后你要确保rsyslog service 运行正常,
		最后调用openlog() 打开该syslog日志文件, 调用syslog 标准的日志API 写入日志数据.





#
# syslog 日志标准宏定义:
#
前置声明:
	一个'用途类型facility', 可以有多个不同的'priorities 优先级';
	'facility'是上级, 'priorities 优先级'是下级.

	3.1 常见facility:
	*** First some standard log files.  Log by facility. ***
		kern              内核信息
		user              用户进程信息
		mail              电子邮件相关信息
		daemon            后台进程相关信息
		authpriv          包括特权信息如用户名在内的认证活动
		cron              计划任务信息
		syslog            系统日志信息
		lpr               打印服务相关信息
		news              新闻组服务器信息
		uucp              uucp 生成的信息
		local0--local7    本地用户信息

		LOG_KERN		(0<<3)	// kernel messages
		LOG_USER		(1<<3)	// random user-level messages
		LOG_MAIL		(2<<3)	// mail system
		LOG_DAEMON	(3<<3)	// system daemons
		LOG_AUTH		(4<<3)	// security/authorization messages
		LOG_SYSLOG	(5<<3)	// messages generated internally by syslogd
		LOG_LPR			(6<<3)	// line printer subsystem
		LOG_NEWS		(7<<3)	// network news subsystem
		LOG_UUCP		(8<<3)	// UUCP subsystem
		LOG_CRON		(9<<3)	// clock daemon
		LOG_AUTHPRIV(10<<3)	// security/authorization messages (private)
		LOG_FTP			(11<<3)	// ftp daemon



	3.2 priorities 优先级(按严重程度由低到高排序):
		debug       不包含函数条件或问题的其他信息
		info        提供信息的消息
		none        没有重要级, 通常用于排错
		notice      具有重要性的普通条件
		warning     预警信息
		err         阻止工具或某些子系统部分功能实现的错误条件
		crit        阻止某些工具或子系统功能实现的错误条件
		alert       需要立即被修改的条件
		emerg       该系统不可用

		LOG_EMERG		0				// system is unusable
		LOG_ALERT		1				// action must be taken immediately
		LOG_CRIT		2				// critical conditions
		LOG_ERR			3				// error conditions
		LOG_WARNING	4				// warning conditions
		LOG_NOTICE	5				// normal but significant condition
		LOG_INFO		6				// informational
		LOG_DEBUG		7				// debug-level messages



	3.3 操作动作, syslog 主要支持以下活动:
		file							指定文件的绝对路径
		terminal 或 prin		完全的串行或并行设备标志符
		@host(@IP地址)			远程的日志服务器

*/





#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>



int main(void){
	int options, facility, priorities;
	const char *log_file_name = "daemon.log";



	//设置日志'用途类型'
	facility = LOG_KERN;			// kernel messages
	facility = LOG_USER;			// random user-level messages
	facility = LOG_MAIL;			// mail system
	facility = LOG_DAEMON;		// system daemons
	facility = LOG_AUTH;			// security/authorization messages
	facility = LOG_SYSLOG;		// messages generated internally by syslogd
	facility = LOG_LPR;				// line printer subsystem
	facility = LOG_NEWS;			// network news subsystem
	facility = LOG_UUCP;			// UUCP subsystem
	facility = LOG_CRON;			// clock daemon
	facility = LOG_AUTHPRIV;	// security/authorization messages (private)
	facility = LOG_FTP;				// ftp daemon

	facility = LOG_DAEMON;		//***最终选出***



	//设置优先级类型(facility 选定后, 这个priorities可以随意切换)
	priorities = LOG_EMERG;		// system is unusable
	priorities = LOG_ALERT;		// action must be taken immediately
	priorities = LOG_CRIT;		// critical conditions
	priorities = LOG_ERR;			// error conditions
	priorities = LOG_WARNING;	// warning conditions
	priorities = LOG_NOTICE;	// normal but significant condition
	priorities = LOG_INFO;		// informational
	priorities = LOG_DEBUG;		// debug-level messages

	//priorities = LOG_ERR;		//***最终选出*** for test
	//												(错误,可以直接填写消息优先级类型,不需要特别说明)



	//设置options(每条日志附带的信息, 例如: LOG_PID = 每条日志附带提交日志的进程pid)
	options |= LOG_PID;				// log the pid with each message
														// 在日志中, 添加所在的进程pid条目

	options |= LOG_CONS;			// log on the console if errors in sending
														// 如果发送错误, 请登录控制台(NO)

	options |= LOG_ODELAY;		// delay open until first syslog() (default)
														// 延迟打开(YES)

	//options |= LOG_NDELAY;	// don't delay open
														// (NO)

	//options |= LOG_NOWAIT;	// don't wait for console forks: DEPRECATED
														// 不要等待控制台分叉:已弃用(NO)

	//options |= LOG_PERROR;	// log to stderr as well
														// 将该日志,抄送到stderr(daemon 程序一般不需要!)



	//最终测试:
	openlog(log_file_name,options,facility);	//打开日志文件
	syslog(LOG_DEBUG,"%s","love me harder");	//刷写一条日志
	syslog(LOG_ERR,"%s","love me harder2");		//刷写一条日志
	syslog(LOG_INFO,"%s","love me harder3");	//刷写一条日志
	closelog();																//关闭日志文件


	exit(EXIT_SUCCESS);
}
