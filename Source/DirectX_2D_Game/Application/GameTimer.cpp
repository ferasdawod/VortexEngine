#include "GameTimer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

GameTimer::GameTimer()
: _nSecondsPerCount(0.0), _nDeltaTime(-1.0), _nBaseTime(0),
_nPauseTime(0), _nPrevTime(0), _nCurrentTime(0), _nStopTime(0),
_bStopped(false), _nTimeScale(1.0f)
{
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	_nSecondsPerCount = 1.0 / static_cast<double>(countsPerSecond);
}

float GameTimer::GameTime() const
{
	if (_bStopped)
	{
		return static_cast<float>(((_nStopTime - _nPauseTime) - _nBaseTime) * _nSecondsPerCount);
	}
	else
	{
		return static_cast<float>(((_nCurrentTime - _nPauseTime) - _nBaseTime) * _nSecondsPerCount);
	}
}

float GameTimer::DeltaTime() const
{
	return static_cast<float>(_nDeltaTime);
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	_nBaseTime = currTime;
	_nPrevTime = currTime;
	_nStopTime = 0;
	_bStopped = false;
}

void GameTimer::Start()
{
	if (_bStopped)
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		_nPauseTime += (startTime - _nStopTime);

		_nPrevTime = startTime;

		_nStopTime = 0;
		_bStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!_bStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		_nStopTime = currTime;
		_bStopped = true;
	}
}

void GameTimer::Tick()
{
	if (_bStopped)
	{
		_nDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	_nCurrentTime = currTime;

	_nDeltaTime = (_nCurrentTime - _nPrevTime) * _nSecondsPerCount;
	_nPrevTime = _nCurrentTime;

	if (_nDeltaTime < 0.0)
		_nDeltaTime = 0.0;
}
