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
	auto f = fopen("fizzbuzz_timings.log", "w+");
	fprintf(f, "Header: TimeLogEntry: request=uint64,startTS=timestamp,finishParsingTS=timestamp,finishProcessingTS=timestamp,finishSendTS=timestamp\n");
	for (auto& entry : mEntries)
	{
		fprintf(f, "Data: TimeLogEntry: request=%llu,startTS=%llu,finishParsingTS=%llu,finishProcessingTS=%llu,finishSendTS=%llu\n",
			entry.request, ToEpoch(entry.start), ToEpoch(entry.finishParsingTSC),
			ToEpoch(entry.finishProcessingTSC), ToEpoch(entry.finishSendTSS));
	}
	fclose(f);
}

}