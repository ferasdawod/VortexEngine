#pragma once

#define PROFILE(name) ScopedProfilerSample __profilerSample((name))
#define FUNC_PROFILE() ScopedProfilerSample __profilerSample(__FUNCTION__)

class ScopedProfilerSample
{
public:
	ScopedProfilerSample(const char* sampleName);
	virtual ~ScopedProfilerSample();

protected:
	const char*		_sSampleName;
};