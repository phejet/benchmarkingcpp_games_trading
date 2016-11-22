#include "timing.h"
#include "stdio.h"

namespace Demo {

TimeLog::TimeLog()
{
	mStartEpochNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
	mStartTSC = rdtsc();

	mEntries.reserve(MAX_TIME_LOG_SIZE);
}

uint64_t TimeLog::ToEpoch(uint64_t tsc)
{
	uint64_t delta = tsc - mStartTSC;
	return mStartEpochNanos.count() + delta;
}

void TimeLog::DumpLog()
{
	// dump profiling output to server_profiling.log
	auto f = fopen("server_programming.log", "w+");
	for (auto& entry : mEntries)
	{
		fprintf(f, "TimeLogEntry: request=%llu,start=%llu,finishParsingTSC=%llu,finishProcessingTSC=%llu,finishSendTSS=%llu\n",
			entry.request, ToEpoch(entry.start), ToEpoch(entry.finishParsingTSC),
			ToEpoch(entry.finishProcessingTSC), ToEpoch(entry.finishSendTSS));
	}
	fclose(f);
}

}