#include "pch.h"

#include "ProfilingSample.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

double ProfilingSample::_nSecondsPerCount = -1.0;

ProfilingSample::ProfilingSample() : _MaxTime(0.0f), _MinTime(0.0f), _nStartTime(0), _nCounter(0), _nSumMilliSeconds(0), _nAvarageTime(0)
{
	if (_nSecondsPerCount < 0.0)
	{
		__int64 countsPerSecond = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
		_nSecondsPerCount = 1.0 / static_cast<double>(countsPerSecond);
	}

	_MinTime = std::numeric_limits<double>::infinity();
	_MaxTime = std::numeric_limits<double>::lowest();
}

void ProfilingSample::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_nStartTime);
}

void ProfilingSample::Stop()
{
	++_nCounter;

	__int64 currentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	auto deltaTime = currentTime - _nStartTime;
	auto durationMilli = deltaTime * _nSecondsPerCount * 1000.0;

	_MaxTime = max(durationMilli, _MaxTime);
	_MinTime = min(durationMilli, _MinTime);

	_nSumMilliSeconds += durationMilli;
}

double ProfilingSample::GetAvarageTime() const
{
	if (_nCounter == 1) return _nSumMilliSeconds;

	if (_nCounter < 3) return _nSumMilliSeconds / static_cast<double>(_nCounter);

	auto sum = _nSumMilliSeconds;
	sum -= _MaxTime;
	sum -= _MinTime;


	return sum / static_cast<double>(_nCounter - 2);
}

