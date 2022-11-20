-- 切换db4adan_srv数据库
use db4adan_srv;

-- 创建uinfo表 -- auto_increment = 自动增长
create table uinfo
(
uid int not null,
uname varchar(16) not null,
password varchar(16) not null,
sex tinyint not null,
age tinyint not null,
email varchar(30) not null,
city varchar(20) not null,
register_date datetime not null,
unique (email),
primary key (uid)
);

-- 创建upicture表
create table upicture
(
uid int not null,
picture varchar(100) not null,
primary key (uid)
);



-- 创建存储过程 -- delimiter 是通知mysql shell 执行delimiter 中的代码
delimiter //

	-- 执行清洗uinfo 表数据-方便测试
	create definer=`adan`@`localhost` procedure uinfo_clean()
	begin
	truncate table uinfo;
	end;

	-- 插入uinfo_add 表
	create definer=`adan`@`localhost` procedure uinfo_add(
	in uid int,
	in uname varchar(16),
	in password varchar(16),
	in sex tinyint,
	in age tinyint,
	in email varchar(30),
	in city varchar(20),
	in register_date datetime
	)
	begin
	set @uid = uid;
	set @uname = uname;
	set @password = password;
	set @sex = sex;
	set @age = age;
	set @email = email;
	set @city = city;
	set @register_date = register_date;
	insert into uinfo(uid,uname,sex,age,email,city,register_date) values(@uid,@uname,@sex,@age,@email,@city,@register_date);
	end;

	-- 查询uinfo 表by uid
	create definer=`adan`@`localhost` procedure uinfo_select_by_uid(
	in uid int
	)
	begin
	set @uid = uid;
	select * from uinfo where uid = @uid;
	end;

	-- 修改uinfo 表byuid
	create definer=`adan`@`localhost` procedure uinfo_change(
	in uid int,
	in uname varchar(16),
	in password varchar(16),
	in sex tinyint,
	in age tinyint,
	in email varchar(30),
	in city varchar(20)
	)
	begin
	set @uid = uid;
	set @uname = uname;
	set @password = password;
	set @sex = sex;
	set @age = age;
	set @email = email;
	set @city = city;
	update uinfo set uname=@uname,sex=@sex,age=@age,email=@email,city=@city where uid = @uid;
	end;

	-- 执行清洗upicture 表数据-方便测试
	create definer=`adan`@`localhost` procedure upicture_clean()
	begin
	truncate table upicture;
	end;

	-- 插入upicture_add 表
	create definer=`adan`@`localhost` procedure upicture_add(
	in uid int,
	in picture varchar(100)
	)
	begin
	set @uid = uid;
	set @picture = picture;
	insert into upicture(uid,picture) values(@uid,@picture);
	end;

	-- 查询upicture 表by uid
	create definer=`adan`@`localhost` procedure upicture_select_by_uid(
	in uid int
	)
	begin
	set @uid = uid;
	select * from upicture where uid = @uid;
	end;

	-- 修改upicture 表byuid
	create definer=`adan`@`localhost` procedure upicture_change(
	in uid int,
	in picture varchar(100)
	)
	begin
	set @uid = uid;
	set @picture = picture;
	update upicture set picture=@picture where uid = @uid;
	end;

//



-- 修改两个表为哈希表 --The used table type doesn't support BLOB/TEXT columns
alter table uinfo engine =heap;
alter table upicture engine =heap;

