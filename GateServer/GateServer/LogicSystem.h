#pragma once
#include "Singleton.h"
#include <functional>
#include <map>
#include "const.h"

//���������඼����const�ظ�����
class HttpConnection;
//Http�������Ļص�����
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandle;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	void RegGet(std::string, HttpHandle handler);
private:
	LogicSystem();
	std::map<std::string, HttpHandle> _post_handlers;
	std::map<std::string, HttpHandle> _get_handlers;
};

