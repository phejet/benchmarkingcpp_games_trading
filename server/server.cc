#include <stdio.h>
#include <cstdlib> 
#include <stdint.h>
#include <string>
#include <type_traits>
#include <assert.h>
#include <chrono>
#include <ctime>
#include <inttypes.h>
#include <vector>
#include "timing.h"

//#include <sys/socket.h>
//#include <netinet/in.h>
#pragma comment (lib, "Ws2_32.lib")


namespace Demo {

class Connection
{
public:
	size_t	RecvBufferSize()
	{
		return 0;
	}
	char*	GetRecvBuffer()
	{
		return nullptr;
	}

	void	Send(const std::string& data)
	{
	}
};

typedef size_t(*RequestHandlerFunc)(Connection*);

template <typename RetType>
RetType ReadInt(char* input, size_t len)
{
	RetType n = 0;
	while (len--)
	{
		assert(*input >= '0' && *input <= '9');
		n = n * 10 + *input - '0';
		input++;
	}

	return n;
}



uint64_t g_SeqNumber = 0;

std::string MakeFizzBuzz(uint64_t number)
{
	std::string output;
	if (number % 3 == 0)
	{
		output += "Fizz";
	}
	if (number % 5 == 0)
	{
		output += "Buzz";
	}
	if (output.empty())
	{
		output = std::to_string(number);
	}
	return output;
}

void HandleRequest(char* recvBuf, size_t recvSize, Connection* conn)
{
	TimeCapture timeCapture(g_timeLog);			// <--- START PROCESSING TS
	uint64_t number = ReadInt<uint64_t>(recvBuf, recvSize);
	timeCapture.CaptureFinishParsingTSC();		// <--- FINISH PARSING TS
	std::string output = MakeFizzBuzz(number);
	timeCapture.CaptureFinishProcessingTSC();	// <--- FINISH PROCESSING TS
	conn->Send(output);
	timeCapture.CaptureFinishSendTSC();			// <--- FINISH SENDING TS
	timeCapture.SetSeqNum(g_SeqNumber);
}


void RunApplication(int port)
{
	;

	printf("Waiting for a connection\n");
	//HandleIncomingConnection(port, HandleRequest);
	//int hsock = socket(AF_INET, SOCK_STREAM, 0);
}



int main(int argc, char *argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
//	auto epoch_time_us = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch());
	auto epoch_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch());
	auto epoch_time_ns2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end.time_since_epoch());

	uint64_t t0 = rdtsc();
	uint64_t t1 = rdtsc();

	printf("start = %lld\n", epoch_time_ns.count());
	printf("end   = %lld\n", epoch_time_ns2.count());

	printf("start TSC = %" PRIu64 "\n", t0);
	printf("end   TSC = %" PRIu64 "\n", t1);
	std::chrono::nanoseconds ms = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

    if (argc < 2)
    {
		printf("Usage:\n\t server port_number\n");
		printf("Please provide port to listen on\n");
		return 0;
    }

    int port = atoi(argv[1]);
	printf("Running ROT1 encryption server on %d port...\n", port);

	RunMainLoop(port);

	printf("Exit\n");
	return 0;
}

}