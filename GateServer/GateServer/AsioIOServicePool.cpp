#include "AsioIOServicePool.h"

AsioIOServicePool::~AsioIOServicePool()
{
	stop();
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

void AsioIOServicePool::stop()
{
	for (auto& work : _works) {
		work->get_io_context().stop();
		work.reset();
	}
	for (auto& t : _thread_pool) {
		t.join();
	}
}

IOService& AsioIOServicePool::GetIOService()
{
	auto& ioc= _ioservice[_nextioservice++];
	if (_nextioservice == _ioservice.size()) {
		_nextioservice = 0;
	}
	return ioc;
}

AsioIOServicePool::AsioIOServicePool(std::size_t size):_nextioservice(0),
_ioservice(size),_works(size)
{
	if (size < 2) {
		size = 2;
	}
	for (auto i = 0; i < size; i++) {
		_works[i] = std::make_unique<Work>(_ioservice[i]);
	}
	for (auto i = 0; i < size; i++) {
		_thread_pool.emplace_back([this,i]() {
			_ioservice[i].run();
			});
	}
}
