#include "timing.h"
#include "stdio.h"

namespace Demo {

TimeLog::TimeLog()
{
	mEntries.reserve(MAX_TIME_LOG_SIZE);
}

uint64_t ToEpoch(Timestamp ts)
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(ts.time_since_epoch()).count();
}

void TimeLog::DumpLog()
{
	// dump profiling output to server_profiling.log
	auto f = fopen("fizzbuzz_timings.log", "w+");
	fprintf(f, "Header: TimeLogEntry: request=uint64,startTS=timestamp,finishParsingTS=timestamp,finishProcessingTS=timestamp,finishSendTS=timestamp\n");
	for (auto& entry : mEntries)
	{
		fprintf(f, "Data: TimeLogEntry: request=%lu,startTS=%lu,finishParsingTS=%lu,finishProcessingTS=%lu,finishSendTS=%lu\n",
			entry.request, ToEpoch(entry.startTS), ToEpoch(entry.finishParsingTS),
			ToEpoch(entry.finishProcessingTS), ToEpoch(entry.finishSendTS));
	}
	fclose(f);
}

}
