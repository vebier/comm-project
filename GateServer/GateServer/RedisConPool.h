#pragma once
#include"const.h"
class RedisConPool
{
public:
	RedisConPool(size_t poolSize, const std::string& host, int port, const std::string& pwd);
	void Close();
	redisContext* GetConnect();
	void RetConnect(redisContext* connect);
	~RedisConPool();
private:
	
	std::atomic<bool> _b_stop;
	std::queue<redisContext*> _connects;
	std::size_t _size;
	int _port;
	const std::string& _host;
	std::mutex _mutex;
	std::condition_variable _cond;
};

