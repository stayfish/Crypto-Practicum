#ifndef RSA_H
#define RSA_H
#include<NTL/ZZ.h>
using namespace NTL;


const int PRIME_LEN1 = 512;
const int PRIME_LEN2 = 1024;

class RSA {
public:
	struct Public_key
	{
		ZZ n;
		ZZ b;
	};
	Public_key pub;//用于加别人密的公钥
public:
	RSA();
	void setPublicKey(Public_key);
	void GenerateKey(int l=PRIME_LEN1);
	ZZ encrypt(ZZ);
	ZZ decrypt(ZZ);
	Public_key GetPublicKey() { return this->key.pub; }
private:
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
	Key key;
};

#endif // !RSA
