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
	DECLARE_PROPERTY_READ_ONLY(__int64, Counter);

	double			GetAvarageTime() const;

private:
	__int64			_nStartTime;

	double			_nAvarageTime;
	long double		_nSumMilliSeconds;

	static double	_nSecondsPerCount;
};