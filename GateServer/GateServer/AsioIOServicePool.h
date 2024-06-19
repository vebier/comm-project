#pragma once
#include"const.h"
#include"Singleton.h"

using IOService = boost::asio::io_context;
using Work = boost::asio::io_context::work;
using WorkPtr = std::unique_ptr<Work>;

class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
	friend class Singleton<AsioIOServicePool>;
public:
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	~AsioIOServicePool();
	void stop();
	IOService& GetIOService();
private:
	AsioIOServicePool(std::size_t size=std::thread::hardware_concurrency());
	std::vector<IOService> _ioservice;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _thread_pool;
	std::size_t _nextioservice;
};

