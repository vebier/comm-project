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
				//�������������
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
	//���ð汾
	_response.version(_request.version());
	//����Ϊ������
	_response.keep_alive(false);
	//����get������Ҫ����url���Լ���PreParseGetParam����
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

	//����post����
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
		//��ԭ+Ϊ��
		if (str[i] == '+') strTemp += ' ';
		//����%������������ַ���16����תΪchar��ƴ��
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
	std::string url = _request.target();//��ȡget�����url
	auto query_pos = url.find('?');
	if (query_pos == std::string::npos)
	{
		_get_url = url;
		return;
	}
	//���ֺ�����key�Լ�value
	//localhost:8080 / get_test ? key1 = value1 & key2 = value2
	_get_url = url.substr(0, query_pos);//get_test
	std::string query_string = url.substr(query_pos+1);//key1 = value1 & key2 = value2
	std::string Key;
	std::string Value;

	size_t pos=0;
	while ((pos = query_string.find('&')) != std::string::npos)//ÿ�λ�ȡһ��key��value
	{
		std::string pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			Key = UrlDecode(pair.substr(0, eq_pos));
			Value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[Key] = Value;
		}
		query_string.erase(0, pos+1);//ɾ����������Key Value
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
