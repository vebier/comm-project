#include "LogicSystem.h"
#include"HttpConnection.h"
LogicSystem::LogicSystem()
{
	//这里调用RegGet然后把lambd表达式作为实参传过去
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->_response.body()) << "receive get_test req" << std::endl;
		int i = 0;
		for (auto& elem : connection->_get_params) {
			i++;
			beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
			beast::ostream(connection->_response.body()) << ", " << " value is " << elem.second << std::endl;
		}
		});

	
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
		
		auto post_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << post_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Value src_root;
		Json::Reader reader;
		bool parse_success = reader.parse(post_str,src_root);
		if (!parse_success)
		{
			std::cout << "Failde to pares JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;//网页打印错误码
			return true;
		}

		
		if (!src_root.isMember("email"))
		{
			std::cout << "Failde to pares JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;//网页打印错误码
			return true;
		}

		auto email = src_root["email"].asString();//以字符串的方式转换
		std::cout << "email is " << email << std::endl;
		root["error"] = 0;
		root["email"] = src_root["email"];
		std::string jsonstr = root.toStyledString();//以Json格式方式转换，打印出来可能是{"email"=XXX,"error"=0,}
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
		});
}



LogicSystem::~LogicSystem()
{
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_get_handlers.find(path) == _get_handlers.end()) {
		return false;
	}
	_get_handlers[path](con);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandle handler)
{
	_get_handlers.insert(std::make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandle handler)
{
	
	_post_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
	
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}
	
	_post_handlers[path](con);
	return true;
	
}

