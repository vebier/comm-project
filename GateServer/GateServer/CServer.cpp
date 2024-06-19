#include "CServer.h"
#include"HttpConnection.h"
#include"AsioIOServicePool.h"

void CServer::start()
{
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSokcet(), [self, new_con](boost::system::error_code ec) {
		try
		{
			if (ec)
			{
				self->start();
				return;
			}
			
			new_con->start();
			self->start();
		
		}
		catch (const std::exception& e)
		{	
			std::cout << "Exception is " << e.what() << std::endl;
			self->start();
		}
	});
}


CServer::CServer(net::io_context& ioc, unsigned short port):_ioc(ioc),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
{
	
}
