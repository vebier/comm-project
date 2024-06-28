#include "SqlPool.h"



SqlPool::SqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int size)
	:_url(url),_user(user),_pass(pass),_schema(schema),_poolsize(size)
{
	try
	{
		for (size_t i = 0; i < _poolsize; i++)
		{
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
			auto con = driver->connect(_url, _user, _pass);
			con->setSchema(_schema);
			//获取一个时间戳
			auto currentime = std::chrono::system_clock::now().time_since_epoch();
			//转换成秒
			long long time = std::chrono::duration_cast<std::chrono::seconds>(currentime).count();
			_pool.push(std::make_unique <SqlConnection>(con, time));
			
			_thread = std::thread([this]() {
				CheckConnection();
				});
			_thread.detach();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "mysql pool init failed is " << e.what() << std::endl;
	}
}

void SqlPool::CheckConnection()
{
	std::unique_lock<std::mutex> lock(_mtx);
	int size = _pool.size();
	//获取一个时间戳
	auto currentime = std::chrono::system_clock::now().time_since_epoch();
	//转换成秒
	long long time = std::chrono::duration_cast<std::chrono::seconds>(currentime).count();
	

	for (int i = 0; i < size; i++) {

		auto con = std::move(_pool.front());
		_pool.pop();
		Defer defer([this, &con]() {
			_pool.push(std::move(con));
			});

		if (time - con->_last_time < 300) {
			continue;
		}
		//如果长时间没有做操作，有可能断开，主动发起一个查询的操作避免连接丢失
		try
		{
			std::unique_ptr<sql::Statement> state(con->_con->createStatement());
			state->executeQuery("SELECT 1");
			con->_last_time = time;
			std::cout << "execute timer alive query , cur is " << time << std::endl;
		}
		catch (const std::exception& e)
		{
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
			auto newcon = driver->connect(_url, _user, _pass);
			newcon->setSchema(_schema);
			con->_con.reset(newcon);
			con->_last_time=time;
		}
	}
}

std::unique_ptr<SqlConnection> SqlPool::GetConection()
{
	std::unique_lock<std::mutex> lock(_mtx);
	_cond.wait(lock, [this]() {
		return _b_stop || !_pool.empty();
		});
	if (_b_stop) {
		return nullptr;
	}
	std::unique_ptr<SqlConnection> con(std::move(_pool.front()));
	_pool.pop();
	return con;
}

void SqlPool::RetConection(std::unique_ptr<SqlConnection> con)
{
	std::unique_lock<std::mutex> lock(_mtx);
	if (_b_stop) {
		return;
	}
	_pool.push(std::move(con));
	_cond.notify_one();
}

void SqlPool::Close()
{
	_b_stop = true;
	_cond.notify_all();
}

SqlPool::~SqlPool()
{
	std::unique_lock<std::mutex> lock(_mtx);
	while (!_pool.empty()) {
		_pool.pop();
	}
}

SqlConnection::SqlConnection(sql::Connection* con, std::int64_t time):
	_con(con), _last_time(time)
{

}
