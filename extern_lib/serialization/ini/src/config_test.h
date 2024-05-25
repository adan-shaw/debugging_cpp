#include <string>



struct info_server{
	std::string srv_ip;
	std::string log_path;
	int srv_port;
	int test_count;
	int test_err_max;
	int pth_io_count;
};

struct info_redis{
	std::string redis_ip;
	int redis_port;
	int redis_dbkey;
};

struct info_mysql{
	std::string mysql_ip;
	std::string mysql_user;
	std::string mysql_pw;
	std::string mysql_dbname;
	int mysql_port;
};

struct multiline{
	std::string another;
	std::string city;
	std::string cn_text;
};



struct G_val_t{
	struct info_server srv;
	struct info_redis redis;
	struct info_mysql mysql;
	struct multiline mline;
};
