#include<string>
#include<iostream>
#include<fstream>
#include<NTL/ZZ.h>

using namespace NTL;
#define PRIME_LEN1 512
#define PRIME_LEN2 1024

#define IS_PRIME 1
#define NOT_PRIME 0

#define PRIME_TEST_ROUND 5

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
bool Miller_Rabin(const ZZ& n, const ZZ& a)
{
	ZZ m;
	m = n - 1;
	long k = MakeOdd(m);
	//std::cout << "m:" << m << ",k:" << k << std::endl;

	ZZ b;
	PowerMod(b, a, m, n);
	if (b % n == 1)
		return IS_PRIME;
	for (long i = 0; i < k; i++)
	{
		if (b % n == -1)
			return IS_PRIME;
		else
			b = (b * b) % n;
	}
	return NOT_PRIME;
}
bool easy_prime_test(const ZZ& n)
{
	PrimeSeq seq;
	long p = seq.next();
	while(p<2000)
	{
		if (n % p == 0)
		{
			if (n == p)
				return IS_PRIME;
			else
				return NOT_PRIME;
		}
		p = seq.next();
	}
	return IS_PRIME;
}
ZZ myGenPrime(int l,ZZ p=ZZ(0))
{
	ZZ n;
	ZZ a;
	bool Miller_Rabin_test;
	RandomLen(n, l-1);
	n = 2 * n - 1;
	while (1)
	{
		n = n + 2;
		if (easy_prime_test(n) == NOT_PRIME)
			continue;//没通过素数检测，重新生成
		Miller_Rabin_test = IS_PRIME;
		for (int i = 1; i <= PRIME_TEST_ROUND; i++)
		{
			RandomBnd(a, n);
			if (Miller_Rabin(n, a) == NOT_PRIME)
			{
				Miller_Rabin_test = NOT_PRIME;
				break;//没通过素数检测，重新生成
			}
		}
		if (Miller_Rabin_test == IS_PRIME)
		{
			if (n != p)//通过了素数检测，判断是否是已经生成的素数
				break;
		}
	}
	return n;
}
Key RSA(int key_len)
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
	bool flag = Miller_Rabin(p, RandomBnd(p));
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
	Key res;
	res.pub = pub;
	res.pri = pri;
	return res;
}
ZZ RSA_En(Public_key pub, ZZ x)
{
	return PowerMod(x, pub.b, pub.n);
}
ZZ RSA_De(Private_key pri, Public_key pub, ZZ y)
{
	return PowerMod(y, pri.a, pub.n);
}