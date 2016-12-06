#pragma once
#include <stdint.h>
#include <vector>
#include <chrono>

namespace Demo {

// can do some variadic magic to wrap it up and make it easier to define new fields with names
// two std tuples with enums to access fields and some sugar syntax will do the trick


typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

struct TimeLogEntry
{
	uint64_t request = 0;
	Timestamp startTS;
	Timestamp finishParsingTS;
	Timestamp finishProcessingTS;
	Timestamp finishSendTS;
};

class TimeLog
{
public:
	TimeLog();

	void	DumpLog();

private:
	const size_t MAX_TIME_LOG_SIZE = 1000000;
	friend class TimeCapture;
	std::vector<TimeLogEntry> mEntries;
};

class TimeCapture
{
public:
	TimeCapture(TimeLog& timelog)
	{
		timelog.mEntries.emplace_back();
		mEntry = &timelog.mEntries.back();

		mEntry->startTS = CaptureTS();
	}

	~TimeCapture()
	{
		// advanced version would use a background thread logger
		// data for logging will be passed up via lock free queue
		// FB folly is a good candidate
	}

	void SetRequest(uint64_t request)
	{
		mEntry->request = request;
	}
	void CaptureFinishParsingTSC()
	{
		mEntry->finishParsingTS = CaptureTS(); 
	}
	void CaptureFinishProcessingTSC()
	{
		mEntry->finishProcessingTS = CaptureTS();
	}

	void CaptureFinishSendTSC()
	{
		mEntry->finishSendTS = CaptureTS();
	}

private:
	Timestamp CaptureTS()
	{
		return std::chrono::high_resolution_clock::now();
	}


	TimeLogEntry* mEntry = nullptr;
};

}
