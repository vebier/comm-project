#pragma once
#include<iostream>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include<unordered_map>
#include "Singleton.h"
#include <functional>
#include<memory>
#include<map>
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>




namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
	Success = 0,
	Error_Json=1001,
	RPCFailed=1002,
};