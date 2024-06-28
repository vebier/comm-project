#pragma once
#include<iostream>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include<unordered_map>
#include "Singleton.h"
#include <functional>
#include<vector>
#include<memory>
#include<thread>
#include<map>
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>
#include<boost/filesystem.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/ini_parser.hpp>
#include<atomic>
#include<mutex>
#include<queue>
#include<condition_variable>
#include"hiredis.h"
#include<cassert>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
	Success = 0,
	Error_Json=1001,	//Json��������
	RPCFailed=1002,		//RPC�������
	VarifyExpired=1003,	//��֤�����
	VarifyCodeErr=1004,	//��֤�����
	UserExist=1005,		//�û��Ѿ�����
	PasswdErr=1006,		//�������
	EmailNotMatch=1007,	//���䲻ƥ��
	PasswdUpFailed=1008,//��������ʧ��
	PasswdInvalid=1009	//�������ʧ��
};

class Defer {
public:
	Defer(std::function<void()> func):_func(func){}
	~Defer() {
		_func();
	}
	std::function<void()> _func;
};

#define CODEPREFIX "code_"

