#include"RSA.h"
#include"RsaSig.h"
#include<iostream>
#include<fstream>
using namespace std;

#define wfile(file,out)	\
	file<<out;

int main()
{
	int len = 512;
	ifstream file("message.txt", ios::in);
	ofstream sign_file("sign.txt", ios::out);
	ofstream sign_file2("sign2.txt", ios::out);
	ofstream ver_file("ver.txt", ios::out);
	ofstream ver_file2("ver2.txt", ios::out);
	if (!sign_file.is_open())
		cout << "sign fail" << endl;
	if (!sign_file2.is_open())
		cout << "sign2 fail" << endl;
	if (!ver_file.is_open())
		cout << "decode fail" << endl;



	ZZ x;
	RsaSig alice;
	RsaSig bob;
	file >> x;
	alice.rsa.GenerateKey();
	bob.rsa.setPublicKey(alice.rsa.GetPublicKey());
	ZZ sign=alice.sig(x);
	ZZ sign2 = sign + 1;
	bool ver=bob.ver(x,sign);
	bool ver2 = bob.ver(x, sign2);

	wfile(sign_file, sign);
	wfile(sign_file2, sign2);
	wfile(ver_file, ver);
	wfile(ver_file2, ver2);

	return 0;
}