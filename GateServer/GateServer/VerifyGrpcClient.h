#pragma once
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"const.h"
#include"Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
	RPConPool(std::size_t size, std::string host, std::string port);
	~RPConPool();
	void Close();
	std::unique_ptr<VarifyService::Stub> GetConnection();
	void returnConnrction(std::unique_ptr<VarifyService::Stub> context);
private:
	std::string _host;
	std::string _port;
	std::condition_variable _cond;
	std::size_t _pool_size;
	std::queue<std::unique_ptr<VarifyService::Stub>> _connections;
	std::mutex _mutex;
	std::atomic<bool> _stop;
};

class VerifyGrpcClient:public Singleton<VerifyGrpcClient>{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email);
private:
	VerifyGrpcClient();
	std::unique_ptr<RPConPool> RPCPool;
};

