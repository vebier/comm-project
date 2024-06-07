#include "CServer.h"
#include"HttpConnection.h"


void CServer::start()
{
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](boost::system::error_code ec) {
		try
		{
			if (ec)
			{
				self->start();
				return;
			}
			
			std::make_shared<HttpConnection>(std::move(self->_socket))->start();
			self->start();
		
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception is " << e.what() << std::endl;
		}
	});
}

CServer::CServer(net::io_context& ioc, unsigned short port):_socket(ioc),_ioc(ioc),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
{
	std::cout << "Server start success, listen on port : " << port << std::endl;
}
