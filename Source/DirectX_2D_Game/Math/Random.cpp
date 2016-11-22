#include "Random.h"

#include <time.h>
#include <limits>

#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   /* constant vector a */
#define CMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
#define CMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

Random::Random()
{
	_nSeed = 1;
	_nSeed_sp = 0;
	mti = CMATH_N + 1;

	Randomize();
}

unsigned int Random::GetRandom(unsigned int n)
{
	unsigned long y;
	static unsigned long mag01[2] = { 0x0, CMATH_MATRIX_A };

	if (n == 0)
		return(0);

	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= CMATH_N)
	{ /* generate N words at one time */
		int kk;

		if (mti == CMATH_N + 1)   /* if sgenrand() has not been called, */
			SetSeed(4357); /* a default initial seed is used   */

		for (kk = 0; kk < CMATH_N - CMATH_M; kk++)
		{
			y = (mt[kk] & CMATH_UPPER_MASK) | (mt[kk + 1] & CMATH_LOWER_MASK);
			mt[kk] = mt[kk + CMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk < CMATH_N - 1; kk++)
		{
			y = (mt[kk] & CMATH_UPPER_MASK) | (mt[kk + 1] & CMATH_LOWER_MASK);
			mt[kk] = mt[kk + (CMATH_M - CMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[CMATH_N - 1] & CMATH_UPPER_MASK) | (mt[0] & CMATH_LOWER_MASK);
		mt[CMATH_N - 1] = mt[CMATH_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= CMATH_TEMPERING_SHIFT_U(y);
	y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
	y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
	y ^= CMATH_TEMPERING_SHIFT_L(y);

	// ET - old engine added one to the result.
	// We almost NEVER wanted to use this function
	// like this.  So, removed the +1 to return a 
	// range from 0 to n (not including n).
	return (y%n);
}

float Random::GetRandom()
{
	unsigned int inf = std::numeric_limits<unsigned int>::infinity();
	float r = (float)GetRandom(inf);
	float divisor = (float)inf;
	return (r / divisor);
}

void Random::SetSeed(unsigned int seed)
{
	/* setting initial seeds to mt[N] using         */
	/* the generator Line 25 of Table 1 in          */
	/* [KNUTH 1981, The Art of Computer Programming */
	/*    Vol. 2 (2nd Ed.), pp102]                  */
	mt[0] = seed & 0xffffffff;
	for (mti = 1; mti < CMATH_N; mti++)
		mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;

	_nSeed = seed;
}

void Random::Randomize()
{
	SetSeed(static_cast<unsigned int>(time(NULL)));
}