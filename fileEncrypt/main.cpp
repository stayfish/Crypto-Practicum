#include"Certificate.h"
#include<NTL/ZZ.h>
#include<sstream>
using namespace std;
using namespace NTL;




int main()
{
	TA ta("Authority");
	TA ta2("fake");
	Client Alice("Alice");
	Client Bob("Bob");
	Client Oscar("Bob");
	//Alice.callCertificate(ta);
	Bob.callCertificate(ta);
	Oscar.callCertificate(ta2);

	cout << "Alice 验证 Bob的证书" << endl;
	if (Alice.verifyCertificate(Bob.getCertificate(), ta))
		cout << "pass" << endl;
	else
		cout << "fail" << endl;

	cout << "Alice 验证 Oscar的证书" << endl;
	if (Alice.verifyCertificate(Oscar.getCertificate(), ta))
		cout << "pass" << endl;
	else
		cout << "fail" << endl;

}