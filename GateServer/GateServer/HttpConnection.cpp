#include "HttpConnection.h"
#include"LogicSystem.h"
HttpConnection::HttpConnection(tcp::socket socket):_socket(std::move(socket))
{
}

void HttpConnection::start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec,
		size_t bytes_transferred) {
			try
			{
				if (ec)
				{
					std::cout << "http read error is " << ec.what() << std::endl;
					return;
				}
				//处理读到的数据
				boost::ignore_unused(bytes_transferred);
				self->HandleReg();
				self->CheckDeadline();
			}
			catch (const std::exception& e)
			{
				std::cout << "Exception is " << e.what() << std::endl;
				return;
			}
		});
}

void HttpConnection::CheckDeadline()
{
	auto self = shared_from_this();
	deadline_.async_wait([self](beast::error_code ec) {
		if (!ec)
		{
			self->_socket.close(ec);
		}
		});
}

void HttpConnection::WriteResponse()
{
	auto self = shared_from_this();
	_response.content_length(_response.body().size());
	http::async_write(
		_socket,
		_response,
		[self](beast::error_code ec, std::size_t)
		{
			self->_socket.shutdown(tcp::socket::shutdown_send, ec);
			self->deadline_.cancel();
		});
}



void HttpConnection::HandleReg()
{
	//设置版本
	_response.version(_request.version());
	//设置为短链接
	_response.keep_alive(false);
	//处理get请求，需要解析url所以加上PreParseGetParam函数
	if (_request.method() == http::verb::get) {
		PreParseGetParam();
		bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}

	//处理post请求
	if (_request.method() == http::verb::post) {
		bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}


}

unsigned char HttpConnection::FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string HttpConnection::UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//还原+为空
		if (str[i] == '+') strTemp += ' ';
		//遇到%将后面的两个字符从16进制转为char再拼接
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

void HttpConnection::PreParseGetParam()
{
	std::string url = _request.target();//获取get请求的url
	auto query_pos = url.find('?');
	if (query_pos == std::string::npos)
	{
		_get_url = url;
		return;
	}
	//发现后面有key以及value
	//localhost:8080 / get_test ? key1 = value1 & key2 = value2
	_get_url = url.substr(0, query_pos);//get_test
	std::string query_string = url.substr(query_pos+1);//key1 = value1 & key2 = value2
	std::string Key;
	std::string Value;

	size_t pos=0;
	while ((pos = query_string.find('&')) != std::string::npos)//每次获取一组key和value
	{
		std::string pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			Key = UrlDecode(pair.substr(0, eq_pos));
			Value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[Key] = Value;
		}
		query_string.erase(0, pos+1);//删除解析过的Key Value
	}
	if (!query_string.empty())
	{
		
		auto eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			Key = UrlDecode(query_string.substr(0, eq_pos));
			Value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[Key] = Value;
		}
	}
}
