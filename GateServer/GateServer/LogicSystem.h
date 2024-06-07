#pragma once
#include "Singleton.h"
#include <functional>
#include <map>
#include "const.h"

//避免两个类都包含const重复引用
class HttpConnection;
//Http传过来的回调函数
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

