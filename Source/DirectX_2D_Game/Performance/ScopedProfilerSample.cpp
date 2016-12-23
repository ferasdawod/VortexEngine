#include "pch.h"

#include "ScopedProfilerSample.h"

#include "ProfilingManager.h"

ScopedProfilerSample::ScopedProfilerSample(const char* sampleName) : _sSampleName(sampleName)
{
	ProfilingManager::GetPtr()->StartSample(sampleName);
}

ScopedProfilerSample::~ScopedProfilerSample()
{
	ProfilingManager::GetPtr()->EndSample(_sSampleName);
}
