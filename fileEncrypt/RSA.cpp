#include<string>
#include<iostream>
#include<fstream>
#include<NTL/ZZ.h>
#include"RSA.h"

using namespace NTL;

#define IS_PRIME 1
#define NOT_PRIME 0

#define PRIME_TEST_ROUND 5


//bool Miller_Rabin(const ZZ& n, const ZZ& a)
//{
//	ZZ m;
//	m = n - 1;
//	long k = MakeOdd(m);
//	//std::cout << "m:" << m << ",k:" << k << std::endl;
//
//	ZZ b;
//	PowerMod(b, a, m, n);
//	if (b % n == 1)
//		return IS_PRIME;
//	for (long i = 0; i < k; i++)
//	{
//		if (b % n == -1)
//			return IS_PRIME;
//		else
//			b = (b * b) % n;
//	}
//	return NOT_PRIME;
//}
//bool easy_prime_test(const ZZ& n)
//{
//	PrimeSeq seq;
//	long p = seq.next();
//	while(p<2000)
//	{
//		if (n % p == 0)
//		{
//			if (n == p)
//				return IS_PRIME;
//			else
//				return NOT_PRIME;
//		}
//		p = seq.next();
//	}
//	return IS_PRIME;
//}
//ZZ myGenPrime(int l,ZZ p=ZZ(0))
//{
//	ZZ n;
//	ZZ a;
//	bool Miller_Rabin_test;
//	RandomLen(n, l-1);
//	n = 2 * n - 1;
//	while (1)
//	{
//		n = n + 2;
//		if (easy_prime_test(n) == NOT_PRIME)
//			continue;//没通过素数检测，重新生成
//		Miller_Rabin_test = IS_PRIME;
//		for (int i = 1; i <= PRIME_TEST_ROUND; i++)
//		{
//			RandomBnd(a, n);
//			if (Miller_Rabin(n, a) == NOT_PRIME)
//			{
//				Miller_Rabin_test = NOT_PRIME;
//				break;//没通过素数检测，重新生成
//			}
//		}
//		if (Miller_Rabin_test == IS_PRIME)
//		{
//			if (n != p)//通过了素数检测，判断是否是已经生成的素数
//				break;
//		}
//	}
//	return n;
//}
RSA::RSA()
{

}
RSA::RSA(const RSA& rsa)
{
	this->key = rsa.key;
	this->pub = rsa.pub;
}
void RSA::operator=(const RSA& rsa)
{
	this->key = rsa.key;
	this->pub = rsa.pub;
}
void RSA::setPublicKey(Public_key pub)
{
	this->pub = pub;
}
void RSA::GenerateKey(int key_len)
{
	ZZ p, q;
	int l;
	try {
		if (key_len != PRIME_LEN1 && key_len != PRIME_LEN2)
			throw std::string("key length is not support");
		else
			l = key_len;
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}

	GenPrime(p, l);
	GenPrime(q, l);
	//bool flag = Miller_Rabin(p, RandomBnd(p));
	//p = myGenPrime(l);
	//q = myGenPrime(l, p);

	ZZ phi;
	mul(phi, p - 1, q - 1);

	ZZ a, b, d;
	do {
		RandomBnd(b,phi);
		GCD(d, b, phi);
	} while (b <= 1 || d != 1);

	InvMod(a, b, phi);
	ZZ n;
	mul(n, p, q);
	Public_key pub = { n,b };
	Private_key pri = { p,q,a };
	this->key.pub = pub;
	this->key.pri = pri;
}
ZZ RSA::encrypt(ZZ x) const 
{
	return PowerMod(x, this->pub.b, this->pub.n);
}
ZZ RSA::decrypt(ZZ y) const
{
	return PowerMod(y, this->key.pri.a, this->key.pub.n);
}