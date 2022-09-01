#ifndef RSA_SIG_H
#define RSA_SIG_H
#include"RSA.h"
#include<NTL/ZZ.h>
using namespace NTL;

class RsaSig{
public:
	bool ver(ZZ, ZZ);
	ZZ sig(ZZ);
public:
	RSA rsa;
};
#endif // !Sig
