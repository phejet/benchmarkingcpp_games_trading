#include "network.h"
#include <stdio.h>

namespace Demo {

void	SimulatedConnection::Send(const std::string& data)
{
	printf("%s\n", data.c_str());
}

void SimulatedServer::Run(const std::vector<SimulatedNetworkPacket>& packets,
	IServerConnectionHandler* handler)
{
	for (const auto& packet : packets)
	{
		handler->HandleRequest(packet.mStr.c_str(), packet.mStr.length(), &mSimConnection);
	}
}


}
