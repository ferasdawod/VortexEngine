#pragma once

class GameTimer
{
public:
	GameTimer();
	virtual ~GameTimer() = default;

	float GameTime() const;
	float DeltaTime() const;

	void	Reset();
	void	Start();
	void	Stop();

	void	Tick();

protected:
	double	_nSecondsPerCount;
	double	_nDeltaTime;

	float	_nTimeScale;

	__int64	_nBaseTime;
	__int64	_nPauseTime;
	__int64	_nStopTime;
	__int64	_nPrevTime;
	__int64	_nCurrentTime;

	bool	_bStopped;
};