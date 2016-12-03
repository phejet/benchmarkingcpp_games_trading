#pragma once
#include <stdint.h>
#include <vector>
#include <chrono>

namespace Demo {

// can do some variadic magic to wrap it up and make it easier to define new fields with names
// two std tuples with enums to access fields and some sugar syntax will do the trick

struct TimeLogEntry
{
	uint64_t request = 0;
	uint64_t start = 0;
	uint64_t finishParsingTSC = 0;
	uint64_t finishProcessingTSC = 0;
	uint64_t finishSendTSS = 0;
};

class TimeLog
{
public:
	TimeLog();

	void	DumpLog();

private:
	uint64_t ToEpoch(uint64_t tsc);

	const size_t MAX_TIME_LOG_SIZE = 4096;
	friend class TimeCapture;
	std::vector<TimeLogEntry> mEntries;
	// some magic to convert captured TSC to UNIX epoch
	std::chrono::nanoseconds mStartEpochNanos;
	uint64_t mStartTSC;
};

static inline unsigned long long rdtsc(void)
{
#ifdef WIN32
	return __rdtsc();
#else
	unsigned long long int x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
#endif
}

class TimeCapture
{
public:
	TimeCapture(TimeLog& timelog)
	{
		timelog.mEntries.emplace_back();
		mEntry = &timelog.mEntries.back();
		mEntry->start = rdtsc();
	}

	~TimeCapture()
	{
		// production version would use background thread logger
		// data for logging will be passed up via lock free queue
		// FB folly is a good candidate
	}

	void SetRequest(uint64_t request)
	{
		mEntry->request = request;
	}
	void CaptureFinishParsingTSC()
	{
		mEntry->finishParsingTSC = rdtsc();
	}
	void CaptureFinishProcessingTSC()
	{
		mEntry->finishProcessingTSC = rdtsc();
	}

	void CaptureFinishSendTSC()
	{
		mEntry->finishSendTSS = rdtsc();
	}

private:
	TimeLogEntry* mEntry = nullptr;
};

}
