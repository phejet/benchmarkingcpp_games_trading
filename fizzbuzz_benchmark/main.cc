#include <stdio.h>
#include <cstdlib> 
#include <stdint.h>
#include <benchmark/benchmark.h>

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
	uint64_t number = ReadInt(recvBuf);
	std::string output = MakeFizzBuzz(number);
	SendReply(output.c_str());
}


std::vector<std::string> PrepareData()
{
	std::vector<std::string> values;
	values.reserve(1000000);
	for (int i = 0; i < 1000000; i++)
	{
		char buf[33];
		sprintf(buf, "%d", i);
		values.push_back(buf);
	}
	return values;
}

static void BM_BaselineFizzBuzz(benchmark::State& state)
{
	auto vals = PrepareData();
	size_t i = 0;
	while (state.KeepRunning())
	{

    	HandleRequest(vals[i].c_str());
    	i++;
    	if (i >= vals.size())
    	{
    		i = 0;
    	}

	}
}

// Register the function as a benchmark
BENCHMARK(BM_BaselineFizzBuzz);

// // Define another benchmark
// static void BM_StringCopy(benchmark::State& state)
// {
// 	std::string x = "hello";
// 	while (state.KeepRunning())
// 	{
//     	std::string copy(x);
// 	}
// }
// BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();