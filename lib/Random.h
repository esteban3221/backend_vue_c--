#include <stdbool.h>
#ifndef _RANDOM_H
#define _RANDOM_H


#define MAX_RANDOM_INTEGER		2147483648LL 
#define MAX_PRIME_NUMBER		2147483648LL 

#define LFSR(n)    {if (n&1) n=((n^0x80000055)>>1)|0x80000000; else n>>=1;}
#define ROT(x, y)  (x=(x<<y)|(x>>(32-y)))

unsigned long long GeneratePrime(void);
bool MillerRabin (long long n, long long trials);
bool IsItPrime (long long n, long long a);
long long XpowYmodN(long long x, long long y, long long N);
unsigned long long GenerateRandomNumber(void);
long long GetRTSC( void );
long long GetSeed( void );



#endif

