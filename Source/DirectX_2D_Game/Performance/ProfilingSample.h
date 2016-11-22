#pragma once

#include "Utilities/Macros.h"

class ProfilingSample
{
public:
	ProfilingSample();
	virtual ~ProfilingSample() {}

	void		Start();
	void		Stop();

	DECLARE_PROPERTY_READ_ONLY(double, MaxTime);
	DECLARE_PROPERTY_READ_ONLY(double, MinTime);

	double			GetAvarageTime() const;

private:
	__int64			_nStartTime;
	__int64			_nCounter;

	double			_nAvarageTime;
	long double		_nSumMilliSeconds;

	static double	_nSecondsPerCount;
};