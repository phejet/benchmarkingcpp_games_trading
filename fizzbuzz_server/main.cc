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
#include <fstream>
#include <unistd.h>


namespace Demo {

struct FizzBuzzer
{
	__attribute__ ((noinline)) 
	int ReadInt(const char* input)
	{
		return std::atoi(input);
	}
	
	__attribute__ ((noinline)) 
	std::string MakeFizzBuzz(uint64_t number)
	{
		std::string out;
		if (number % 3 == 0)
		{
			out = "Fizz";
		}
		if (number % 5 == 0)
		{
			out = "Buzz";
		}
		if (out.empty())
		{
			out = std::to_string(number);
		}
		return out;
	}

	__attribute__ ((noinline)) 
	void	SendReply(const char* data)
	{
		(void)data;
	}

	__attribute__ ((noinline)) 
	void HandleRequest(const char* recvBuf)
	{
		TimeCapture timeCapture(mTimingLog);		// <--- START TS
		uint64_t number = ReadInt(recvBuf);
		timeCapture.CaptureFinishParsingTSC();		// <--- FINISH PARSING TS

		std::string output = MakeFizzBuzz(number);
		timeCapture.CaptureFinishProcessingTSC();	// <--- FINISH PROCESSING TS

		SendReply(output.c_str());
		timeCapture.CaptureFinishSendTSC();			// <--- FINISH SENDING TS

		timeCapture.SetRequest(number);
	}

	std::vector<std::string> PrepareData(int num)
	{
		std::vector<std::string> values;
		values.reserve(num);
		for (int i = 0; i < num; i++)
		{
			char buf[33];
			sprintf(buf, "%d", i);
			values.push_back(buf);
		}
		return values;
	}

	void SimulationRun(const std::vector<std::string>& testValues)
	{
		for (const auto& val : testValues)
		{
			HandleRequest(val.c_str());
			usleep(100);
		}
		mTimingLog.DumpLog();		
	}

	TimeLog mTimingLog;
};

}

int main(int argc, char *argv[])
{
	Demo::FizzBuzzer fizzBuzzer;

    if (argc == 2)
    {
    	std::vector<std::string> packets;
	 	packets.reserve(10000000);

		std::ifstream infile(argv[1]);
		if (infile)
		{
			std::string number;
			int delayNs = 0;
	 		while (infile >> number >> delayNs)
	 		{
	 			packets.emplace_back(number);
	 		}
	 		infile.close();
	 	}
		fizzBuzzer.SimulationRun(packets);
	}
	else
	{
		std::vector<std::string> testValues = fizzBuzzer.PrepareData(30000);
		fizzBuzzer.SimulationRun(testValues);
	}

	return 0;
}
