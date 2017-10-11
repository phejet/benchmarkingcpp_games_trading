#include <stdio.h>
#include <cstdlib> 
#include <stdint.h>
#include <benchmark/benchmark.h>

__attribute__ ((noinline)) 
const char* MakeFizzBuzz(const char* input)
{
	int number = std::atoi(input);
	const bool isFizz = (number % 3) == 0;
	const bool isBuzz = (number % 5) == 0;
	if (isFizz && isBuzz)
	{
		return "FizzBuzz";
	}
	if (isFizz)
	{
		return "Fizz";
	}
	if (isBuzz)
	{
		return "Buzz";
	}
	return input;
}

__attribute__ ((noinline)) 
void	SendReply(const char* data)
{
	(void)data;
}

__attribute__ ((noinline)) 
void HandleRequest(const char* recvBuf)
{
	const char* output= MakeFizzBuzz(recvBuf);
	SendReply(output);
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