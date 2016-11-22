#pragma once

#define PROFILE(name) ScopedProfilerSample __profilerSample((name))

class ScopedProfilerSample
{
public:
	ScopedProfilerSample(const char* sampleName);
	virtual ~ScopedProfilerSample();

protected:
	const char*		_sSampleName;
};