#include "VerifyGrpcClient.h"
#include"ConfigMgr.h"

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email)
{
	ClientContext context;
	GetVarifyRsp Reply;
	GetVarifyReq Request;
	Request.set_email(email);
	auto _stub = RPCPool->GetConnection();
	Status status = _stub->GetVarifyCode(&context, Request, &Reply);
	if (status.ok()) {
		RPCPool->returnConnrction(std::move(_stub));
		return Reply;
	}
	else {
		RPCPool->returnConnrction(std::move(_stub));
		Reply.set_error(ErrorCodes::RPCFailed);
		return Reply;
	}
}

VerifyGrpcClient::VerifyGrpcClient()
{
	auto& gCfgMgr = ConfigMgr::GetIns();
	std::string host = gCfgMgr["VarifyServer"]["Host"];
	std::string port = gCfgMgr["VarifyServer"]["Port"];
	RPCPool.reset(new RPConPool(5, host, port));
}

RPConPool::RPConPool(std::size_t size, std::string host, std::string port):_host(host)
,_port(port),_stop(false), _pool_size(size)
{
	for (auto i = 0; i < size; i++) {
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host+":"+port,
			grpc::InsecureChannelCredentials());
		_connections.push(VarifyService::NewStub(channel));
	}
}

RPConPool::~RPConPool()
{
	std::lock_guard<std::mutex> lock(_mutex);
	Close();
	while (!_connections.empty()) {
		_connections.pop();
	}
}

void RPConPool::Close()
{
	_stop.store(true);
	_cond.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPConPool::GetConnection()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this]() {
		if (_stop == true) return true;
		return !_connections.empty();
		});
	if (_stop) {
		return  nullptr;
	}
	auto context = std::move(_connections.front());
	_connections.pop();
	return context;
}

void RPConPool::returnConnrction(std::unique_ptr<VarifyService::Stub> context)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_stop)return;
	_connections.push(std::move(context));
	_cond.notify_one();
}


