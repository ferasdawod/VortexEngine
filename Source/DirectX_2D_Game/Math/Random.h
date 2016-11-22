#pragma once

#define CMATH_N 624

class Random
{
public:
	Random();

	unsigned int	GetRandom(unsigned int n);
	float			GetRandom();

	void			SetSeed(unsigned int seed);
	unsigned int	GetSeed() const { return _nSeed; }

	void			Randomize();

private:
	unsigned int	_nSeed;
	unsigned int	_nSeed_sp;
	unsigned long	mt[CMATH_N];
	int				mti;
};