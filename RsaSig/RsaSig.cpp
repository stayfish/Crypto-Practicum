#include"RsaSig.h"

bool RsaSig::ver(ZZ x, ZZ y)
{
	return x == this->rsa.encrypt(y);
}
ZZ RsaSig::sig(ZZ x)
{
	return this->rsa.decrypt(x);
}