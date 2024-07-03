#pragma once
#include"const.h"
#include"SqlPool.h"

struct UserInfo {
	std::string name;
	std::string pwd;
	int uid;
	std::string user;
};

class MysqlDao
{
public:
	MysqlDao();
	~MysqlDao();
	int RegUser(const std::string& name,const std::string& email,const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePwd(const std::string& name, const std::string& newpwd);
private:
	
	std::unique_ptr<SqlPool> _pool;
};

