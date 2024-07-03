#pragma once
#include"const.h"
#include<jdbc/mysql_driver.h>
#include<jdbc/mysql_connection.h>
#include<jdbc/cppconn/prepared_statement.h>
#include<jdbc/cppconn/resultset.h>
#include<jdbc/cppconn/statement.h>
#include<jdbc/cppconn/exception.h>
class SqlConnection {
public:
	SqlConnection(sql::Connection* con, std::int64_t time);
	std::unique_ptr<sql::Connection> _con;
	std::int64_t _last_time;
};

class SqlPool
{
public:
	void CheckConnection();//判断连接是否超时
	std::unique_ptr<SqlConnection> GetConection();
	void RetConection(std::unique_ptr<SqlConnection> con);
	void Close();
	~SqlPool();
	SqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int size);
private:
	
	std::string _url;//连接数据库的ip
	std::string _user;//用户
	std::string _pass;//密码
	std::string _schema;//连接选择连接的数据库名字
	std::queue<std::unique_ptr<SqlConnection>> _pool;
	std::condition_variable _cond;
	std::atomic<bool> _b_stop;
	std::thread _thread;
	std::mutex _mtx;
	int _poolsize;
};

