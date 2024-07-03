#pragma once
#include "const.h"

//���������඼����const�ظ�����
class HttpConnection;
//Http get�Լ�post�ص�����
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandle;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	void RegGet(std::string, HttpHandle handler);
	void RegPost(std::string, HttpHandle handler);
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
private:
	LogicSystem();
	std::map<std::string, HttpHandle> _post_handlers;
	std::map<std::string, HttpHandle> _get_handlers;
};

