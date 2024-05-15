
//#include "stdafx.h"
#include "../inc/itl_types.h"
#include <stdlib.h>
#include "Random.h"


unsigned long long GeneratePrime(void)
{
	unsigned long long tmp = 0;

	tmp	=  GenerateRandomNumber();
	tmp	%= MAX_PRIME_NUMBER;

	if ((tmp & 1)==0)
		tmp += 1;

	if (MillerRabin(tmp,5)==true) return tmp;
	do
	{
		tmp+=2;
	} while (MillerRabin(tmp,5)==false);

	return tmp;
}


bool MillerRabin (long long n, long long trials)
{
	long long a = 0;
    long long i;
	for (i=0; i<trials; i++)
	{
		a = (rand() % (n-3))+2;

		if (IsItPrime (n,a)==false)
		{
			return false;
		}
	} return true; 
}



bool IsItPrime (long long n, long long a)
{
	long long d = XpowYmodN(a, n-1, n);
	if (d==1)
		return true;
	else
		return false;

}

long long XpowYmodN(long long x, long long y, long long N)
{
    int i;
	long long result = 1;

	const long long oneShift63 = (( long long) 1) << 63;

    if (y==1) return (x % N);

	for (i = 0; i < 64; y <<= 1, i++){
		result = result * result % N;
		if (y & oneShift63)
			result = result * x % N;
	}

	return result;

}

unsigned long long GenerateRandomNumber(void)
{
	unsigned long rnd = 0x41594c49;
	unsigned long x   = 0x94c49514;
	long long  n;
	unsigned long long ret;


	LFSR(x);

	n = GetRTSC();

	rnd ^= n^x;

	ROT(rnd,7);

	ret = (unsigned long long)GetRTSC() + (unsigned long long)rnd;

	return ret;
}


long long GetSeed( void )
{
// arm32 or arm64 architectures
#if defined __arm__ || defined __aarch64__
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	long microsecond_time = (currentTime.tv_sec * (int)1e6 + currentTime.tv_usec) % __LONG_MAX__;
	long long seed = microsecond_time*getpid() % __LONG_LONG_MAX__;
	return seed;
#else
	long long result;
	asm ("RDTSC" : "=A" (result));
	return result;
#endif
}

long long GetRTSC( void )
{
// arm32 or arm64 architectures
#if defined __arm__ || defined __aarch64__
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	long microsecond_time = (currentTime.tv_sec * (int)1e6 + currentTime.tv_usec) % __LONG_MAX__;
	long long seed = microsecond_time*getpid() % __LONG_LONG_MAX__;
	return seed;
#else
	long long result;
	asm ("RDTSC" : "=A" (result));
	return result;
#endif
}

