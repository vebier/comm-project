#pragma once
#include"const.h"

class HttpConnection :public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
	
public:
	HttpConnection(boost::asio::io_context& ioc);
	void start();
	tcp::socket& GetSokcet();
private:
	void CheckDeadline();
	void WriteResponse();
	void HandleReg();

	//¥¶¿Ìget«Î«Û
	void PreParseGetParam();
	unsigned char FromHex(unsigned char x);
	std::string UrlEncode(const std::string& str);
	std::string UrlDecode(const std::string& str);
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;

	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	net::steady_timer deadline_{ _socket.get_executor(),std::chrono::seconds(60) };
};
