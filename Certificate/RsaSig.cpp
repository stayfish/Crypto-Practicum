#include"RsaSig.h"

bool RsaSig::ver(ZZ x, ZZ y) const
{
	return x == this->rsa.encrypt(y);
}
ZZ RsaSig::sig(ZZ x) const
{
	return this->rsa.decrypt(x);
}