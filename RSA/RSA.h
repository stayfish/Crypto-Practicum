#ifndef RSA1
#define RSA1
#include<NTL/ZZ.h>
using namespace NTL;



struct Public_key
{
	ZZ n;
	ZZ b;
};
struct Private_key
{
	ZZ p;
	ZZ q;
	ZZ a;
};
struct Key
{
	Public_key pub;
	Private_key pri;
};
Key RSA(int);
ZZ RSA_En(Public_key, ZZ);
ZZ RSA_De(Private_key, Public_key, ZZ);
#endif // !RSA
