#include<iostream>
#include<NTL/ZZ.h>
#include<fstream>
#include"RSA.h"

#include<bitset>

using namespace std;
using namespace NTL;

ZZ str2ZZ(const string& str)
{
	ZZ res;
	int len = str.length();
	for (int i = 0; i < len; i++)
	{
		res = (res << 8) + str[i];
	}
	return res;
}
string ZZ2str(const ZZ& num)
{
	ZZ n = num;
	string res;
	char ch;
	while (n != 0)
	{
		ch = n % 256;
		RightShift(n, n, 8);
		res = ch + res;
	}
	return res;
}


int main()
{
	int len = 512;
	ifstream file("message.txt", ios::in);
	ofstream cipher_text("cipher.txt", ios::out);
	ofstream decode_file("decode.txt", ios::out);
	ofstream key_file("key.txt", ios::out);
	if (!cipher_text.is_open())
		cout << "cipher fail" << endl;
	if (!decode_file.is_open())
		cout << "decode fail" << endl;
	if (!key_file.is_open())
		cout << "key fail" << endl;
	string str;
	Key k;
	ZZ cipher;
	ZZ message;
	file >> message;
	k = RSA(len);
	key_file << "public key:" << endl;
	key_file << "n,b" << endl;
	key_file << k.pub.n << "," << k.pub.b << endl;
	key_file << "private key" << endl;
	key_file << "p,q,a" << endl;
	key_file << k.pri.p << "," << k.pri.q << "," << k.pri.a << endl;
	cout << "private key" << endl;
	cout << k.pri.p << "," << k.pri.q << "," << k.pri.a << endl;
	cout << "public key:" << endl;
	cout << k.pub.n << "," << k.pub.b << endl;
	cipher = RSA_En(k.pub, message);
	cipher_text << cipher;
	message = RSA_De(k.pri, k.pub, cipher);
	decode_file << message;
}