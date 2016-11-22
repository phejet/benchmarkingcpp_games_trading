#pragma once

#include <string>
#include <vector>

namespace Demo {

class IConnection
{
public:
	virtual ~IConnection() = default;
	virtual void	Send(const std::string& data) = 0;
};

class SimulatedConnection : public IConnection
{
public:
	void	Send(const std::string& data) override;
};


struct SimulatedNetworkPacket
{
	SimulatedNetworkPacket(const std::string& str, uint64_t delay):
		mStr(str), mDelay(delay) {}

	std::string mStr;
	uint64_t mDelay;
};

class IServerConnectionHandler
{
public:
	virtual void HandleRequest(const char* recvBuf, size_t recvSize, IConnection* conn) = 0;
};

class SimulatedServer
{
public:
	void Run(const std::vector<SimulatedNetworkPacket>& packets, IServerConnectionHandler* handler);

private:
	SimulatedConnection mSimConnection;
};

}