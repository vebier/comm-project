#include "RedisConPool.h"

void RedisConPool::Close()
{
	_b_stop.store(true);
	_cond.notify_all();

}

redisContext* RedisConPool::GetConnect()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this]() {
		if (_b_stop) {
			return false;
		}
		return!_connects.empty();
		});
	if (_b_stop) {
		return nullptr;
	}
	auto context = _connects.front();
	_connects.pop();
	return context;
}

void RedisConPool::RetConnect(redisContext* context)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (_b_stop) {
		return;
	}
	_connects.push(context);
	_cond.notify_one();
}

RedisConPool::~RedisConPool()
{
	std::unique_lock<std::mutex> lock(_mutex);
	while (!_connects.empty()) {
		_connects.pop();
	}
}

RedisConPool::RedisConPool(size_t poolSize, const std::string& host, int port, const std::string& pwd):
_host(host),_port(port),_size(poolSize),_b_stop(false)
{
	for (std::size_t i = 0; i < _size; i++) {
		auto* context = redisConnect(host.c_str(), port);
		if (context == nullptr || context->err != 0) {
			
			if (context == nullptr) {
				
				redisFree(context);
			}
			continue;
		}
		auto Reply = (redisReply*)redisCommand(context, "AUTH %s", pwd.c_str());

		if (Reply->type == REDIS_REPLY_ERROR) {
			std::cout << "认证失败" << std::endl;
			freeReplyObject(Reply);
			redisFree(context);
			continue;
		}
		freeReplyObject(Reply);
		std::cout << "认证成功" << std::endl;
		_connects.push(context);
	}
}
