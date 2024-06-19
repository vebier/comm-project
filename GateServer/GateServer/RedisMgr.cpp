#include "RedisMgr.h"
#include"ConfigMgr.h"



RedisMgr::~RedisMgr()
{
	Close();
}

bool RedisMgr::Get(const std::string& key,  std::string& value)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "GET %s", key.c_str());
	if (_reply == NULL) {
		std::cout << "[ GET  " << key << " ] failed" << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	if (_reply->type != REDIS_REPLY_STRING) {
		std::cout << "[ GET  " << key << " ] failed" << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	value = _reply->str;
	freeReplyObject(_reply);
	std::cout << "Succeed to execute command [ GET " << key << "  ]" << std::endl;
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::Set(const std::string& key, const std::string& value)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "SET %s %s", key.c_str(),value.c_str());
	if (_reply == nullptr) {
		std::cout << "Execut command [ SET " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}

	if (!(_reply->type == REDIS_REPLY_STATUS && (strcmp(_reply->str, "OK") == 0 || strcmp(_reply->str, "OK") == 0))) {
		std::cout << "Execut command [ SET " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}


	freeReplyObject(_reply);
	std::cout << "Execute command [ SET " << key << " " <<value<< " ] success !" << std::endl;
	_pool->RetConnect(_connect);
	return true;
}



bool RedisMgr::LPush(const std::string& key, const std::string& value)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "LPUSH %s %s", key.c_str(), value.c_str());
	if (_reply == nullptr) {
		std::cout << "Execut command [ LPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	if (_reply->type != REDIS_REPLY_INTEGER || _reply->integer <= 0) {
		std::cout << "Execut command [ LPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	freeReplyObject(_reply);
	std::cout << "Execute command [ LPUSH " << key << " " << value << " ] success !" << std::endl;
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::LPop(const std::string& key, std::string& value)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "LPOP %s", key.c_str());
	if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut command [ LPOP " << key << " ] failure !" << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	value = _reply->str;
	freeReplyObject(_reply);
	std::cout << "Execut command [ LPOP " << key << " ] sucess !" << std::endl;
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::RPush(const std::string& key, const std::string& value) {
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "RPUSH %s %s", key.c_str(), value.c_str());
	if (NULL == _reply)
	{
		std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	if (_reply->type != REDIS_REPLY_INTEGER || _reply->integer <= 0) {
		std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(_reply);
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::RPop(const std::string& key,  std::string& value) {
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "RPOP %s ", key.c_str());
	if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut command [ RPOP " << key << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	value = _reply->str;
	std::cout << "Execut command [ RPOP " << key << " ] success ! " << std::endl;
	freeReplyObject(_reply);
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::HSet(const std::string& key, const std::string& hkey, const std::string& value) {
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str());
	if (_reply == nullptr || _reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(_reply);
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	
	const char* argv[4];
	size_t argvlen[4];
	argv[0] = "HSET";
	argvlen[0] = 4;
	argv[1] = key;
	argvlen[1] = strlen(key);
	argv[2] = hkey;
	argvlen[2] = strlen(hkey);
	argv[3] = hvalue;
	argvlen[3] = hvaluelen;
	auto _reply = (redisReply*)redisCommandArgv(_connect, 4, argv, argvlen);
	if (_reply == nullptr || _reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] success ! " << std::endl;
	freeReplyObject(_reply);
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::HGet(const std::string &key, const std::string &hkey,std::string& value)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
    const char* argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();
    auto _reply = (redisReply*)redisCommandArgv(_connect, 3, argv, argvlen);
    if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
        freeReplyObject(_reply);
        std::cout << "Execut command [ HGet " << key << " "<< hkey <<"  ] failure ! " << std::endl;
        return false;
    }
    value = _reply->str;
    freeReplyObject(_reply);
    std::cout << "Execut command [ HGet " << key << " " << hkey << " ] success ! " << std::endl;
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::Del(const std::string& key)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "DEL %s", key.c_str());
	if (_reply == nullptr || _reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ DEL " << key << " ] failure ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	std::cout<< "Execut command [ DEL " << key << " ] success ! " << std::endl;
	freeReplyObject(_reply);
	_pool->RetConnect(_connect);
	return true;
}

bool RedisMgr::ExistsKey(const std::string& key)
{
	auto _connect = this->_pool->GetConnect();
	if (_connect == nullptr) {
		return false;
	}
	auto _reply = (redisReply*)redisCommand(_connect, "exists %s", key.c_str());
	if (_reply == nullptr || _reply->type != REDIS_REPLY_INTEGER || _reply->integer == 0) {
		std::cout << "Not Found [ Key " << key << " ]  ! " << std::endl;
		freeReplyObject(_reply);
		return false;
	}
	std::cout << " Found [ Key " << key << " ] exists ! " << std::endl;
	freeReplyObject(_reply);
	_pool->RetConnect(_connect);
	return true;
}

void RedisMgr::Close()
{
	_pool->Close();
}

RedisMgr::RedisMgr()
{
	auto& gCfgMgr = ConfigMgr::GetIns();
	auto host = gCfgMgr["Redis"]["Host"];
	auto port = gCfgMgr["Redis"]["Port"];
	auto pwd = gCfgMgr["Redis"]["Passwd"];
	_pool = std::make_unique<RedisConPool>(5, host, atoi(port.c_str()), pwd);
}
