#include "LogicSystem.h"
#include"HttpConnection.h"
LogicSystem::LogicSystem()
{
	//�������RegGetȻ���lambd���ʽ��Ϊʵ�δ���ȥ
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
			beast::ostream(connection->_response.body()) << jsonstr;//��ҳ��ӡ������
			return true;
		}

		
		if (!src_root.isMember("email"))
		{
			std::cout << "Failde to pares JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;//��ҳ��ӡ������
			return true;
		}

		auto email = src_root["email"].asString();//���ַ����ķ�ʽת��
		std::cout << "email is " << email << std::endl;
		root["error"] = 0;
		root["email"] = src_root["email"];
		std::string jsonstr = root.toStyledString();//��Json��ʽ��ʽת������ӡ����������{"email"=XXX,"error"=0,}
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

