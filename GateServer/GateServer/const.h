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
	Error_Json=1001,
	RPCFailed=1002
};

