#ifndef CERTIFICATE_H
#define CERTIFICATE_H
#include<string>
#include<vector>
#include<sstream>
#include"RsaSig.h"
using namespace std;

class TA;
class Client {
	friend class TA;
public:
	Client() {}
	Client(const string& id) { ID = id; }
	void operator=(const Client&);
	string getID() const { return ID; }

	void callCertificate(const TA&);
	//bool verifyCertificate(Client, const TA&);
	bool verifyCertificate(const string&, const TA&);
	string getCertificate() const { return Certificate; }
	void writeLog(const string&, const string&, bool);
public:
	RsaSig sig;
private:
	string Certificate;
	string ID;
};

class TA {
public:
	TA() {};
	TA(const string&);
	TA(int, const string&);
	void operator=(const TA&);
	string makeCertificate(Client&) const;
	string getID() const { return ID; }
	Public_key getPubTA() const { return this->sig.rsa.GetPublicKey(); }
private:
	RsaSig sig;
	string ID;
	int primeLen;
};

string ZZ2str(const ZZ&);
#endif // CERTIFICATE_H