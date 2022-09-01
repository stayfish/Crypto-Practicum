#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include"AES.h"
#include"const.h"
#include"F256.h"
#include"CBC.h"
using namespace std;

streampos filesize(string path)
{
	ifstream f(path, ios::in);
	streampos beg = f.tellg();
	f.seekg(0, ios::end);
	streampos end = f.tellg();
	f.close();
	return end - beg;
}


string getPath(string s)
{
	cout << "input the path of " << s << " is:";
	string path;
	cin >> path;
	return path;
}

void AEStest()
{
	//AES test
	cout << "This is the test for AES algorithm" << endl;
	ifstream fm, fk;
	ofstream fc, fh;

	Key seed;
	vector<char> message(16);

	fm.open("message.txt", ios::in);
	if (fm.is_open())
		cout << "open fm successfully" << endl;

	for (int i = 0; i < 16; i++)
	{
		int temp;
		fm >> hex >> temp;
		message[i] = (char)temp;
	}
	fk.open("key.txt", ios::in);

	if (fk.is_open())
		cout << "open fk successfully" << endl;

	for (int i = 0; i < 16; i++)
	{
		int temp;
		fk >> hex >> temp;
		seed[i] = (char)temp;
	}
	vector<char> code = AES(message, seed);

	fc.open("code_ch.txt", ios::out);
	if (fc.is_open())
		cout << "open code_ch successfully" << endl;

	for (auto ch : code)
		fc << ch;
	fh.open("code_hex.txt", ios::out);
	if (fh.is_open())
		cout << "open code_hex successfully" << endl;
	for (char ch : code)
		fh << hex << setw(2) << setfill('0') << ((unsigned int)ch & 0xff) << ' ';

	cout << "code_hex is:" << endl;
	for (char ch : code)
		cout << hex << setw(2) << setfill('0') << ((unsigned int)ch & 0xff) << ' ';
	cout << endl;

	//inv test
	cout << endl;
	cout << "是否测试Inv的组件?(1/0)" << endl;
	bool flag;
	cin >> flag;
	if (flag)
		InvTest(code, seed);
}

void CBCtest()
{
	ifstream fk;
	Key seed;

	fk.open("key2.txt", ios::in);

	if (fk.is_open())
		cout << "open fk successfully" << endl;

	for (int i = 0; i < 16; i++)
	{
		int temp;
		fk >> hex >> temp;
		seed[i] = (char)temp;
	}
	cout << endl;
	cout << "cbc mode test" << endl;
	ifstream fcbcm("CBCMessage.txt", ios::in);
	if (fcbcm.is_open())
		cout << "open CBCMessage successfully" << endl;
	ofstream fcbce("CBCEncryption.txt", ios::out);
	if (fcbce.is_open())
		cout << "open CBCEncryption successfully" << endl;

	ofstream fcbcd("CBCDecryption.txt", ios::out);
	if (fcbcd.is_open())
		cout << "open CBCDecryption successfully" << endl;

	vector<char> messageCBC;
	while (fcbcm.peek() != EOF)
	{
		fcbcm.unsetf(ios::skipws);
		char temp;
		fcbcm.get(temp);
		messageCBC.push_back((char)temp);
	}

	vector<char> codeCBC = CBCEncryption(messageCBC, seed);
	for (auto ch : codeCBC)
		fcbce << ch;

	messageCBC = CBCDecryption(codeCBC, seed);
	for (auto ch : messageCBC)
		fcbcd << ch;
	cout << endl;
	cout << "done" << endl;
}

void AESEncrption(string src_path, string key_path, string dst_path)
{
	Key seed;
	vector<char> message(16);

	ifstream fm, fk;
	ofstream fc;

	fm.open(src_path, ios::in);
	if (fm.is_open())
		cout << "open the message file successfully" << endl;

	int cnt = 0;
	while (cnt<16)
	{
		fm.unsetf(ios::skipws);
		char temp;
		fm.get(temp);
		message[cnt] = (char)temp;
		cnt++;
	}

	fk.open(key_path, ios::in);
	if (fk.is_open())
		cout << "open the key file successfully" << endl;

	cnt = 0;
	while (cnt<16)
	{
		fk.unsetf(ios::skipws);
		char temp;
		fk.get(temp);
		seed[cnt] = (char)temp;
		cnt++;
	}
	vector<char> code = AES(message, seed);

	fc.open(dst_path, ios::out);
	if (fc.is_open())
		cout << "open ciphertext successfully" << endl;

	for (auto ch : code)
		fc << ch;
	cout << endl;
	cout << endl;
	cout << "done" << endl;
}

void AESDecrption(string src_path, string key_path, string dst_path)
{
	Key seed;
	vector<char> ciphertext(16);

	ifstream fc, fk;
	ofstream fm;

	fc.open(src_path, ios::in);
	if (fc.is_open())
		cout << "open the message file successfully" << endl;

	int cnt = 0;
	while (cnt < 16)
	{
		fc.unsetf(ios::skipws);
		char temp;
		fc.get(temp);
		ciphertext[cnt] = (char)temp;
		cnt++;
	}

	fk.open(key_path, ios::in);
	if (fk.is_open())
		cout << "open the key file successfully" << endl;

	cnt = 0;
	while (cnt < 16)
	{
		fk.unsetf(ios::skipws);
		char temp;
		fk.get(temp);
		seed[cnt] = (char)temp;
		cnt++;
	}
	vector<char> code = AESInv(ciphertext, seed);

	fm.open(dst_path, ios::out);
	if (fm.is_open())
		cout << "open ciphertext successfully" << endl;

	for (auto ch : code)
		fm << ch;
	cout << endl;
	cout << endl;
	cout << "done" << endl;
}

void CBCEncryption(string src_path, string key_path, string dst_path)
{
	ifstream fk;
	Key seed;

	int cnt = 0;

	ifstream fcbcm(src_path, ios::in);
	if (fcbcm.is_open())
		cout << "open message file successfully" << endl;

	ofstream fcbcc(dst_path, ios::out);
	if (fcbcc.is_open())
		cout << "open ciphertext file successfully" << endl;

	fk.open(key_path, ios::in);
	if (fk.is_open())
		cout << "open key file successfully" << endl;
	cnt = 0;
	while (cnt < 16)
	{
		fk.unsetf(ios::skipws);
		char temp;
		fk.get(temp);
		seed[cnt] = (char)temp;
		cnt++;
	}

	vector<char> messageCBC;
	while (fcbcm.peek() != EOF)
	{
		fcbcm.unsetf(ios::skipws);
		char temp;
		fcbcm.get(temp);
		messageCBC.push_back((char)temp);
	}

	vector<char> codeCBC = CBCEncryption(messageCBC, seed);
	int ct = 0;
	for (auto ch : codeCBC)
		cout << ct++ << " " << hex << ((unsigned int)ch&0xff) << endl;
	for (auto ch : codeCBC)
		fcbcc << (unsigned char)ch;


	ofstream fiv("./IV.txt", ios::out);
	string iv = getIV();
	cout << "IV: ";
	for (auto ch : iv)
		cout << ((unsigned int)ch & 0xff) << " ";
	for (auto ch : iv)
		fiv << (unsigned char)ch;




	cout << endl;
	cout << "done" << endl;
}

void CBCDecryption(string src_path, string key_path, string dst_path,string iv_path)
{
	ifstream fk;
	Key seed;

	int cnt = 0;

	ifstream fcbcc(src_path, ios::in);
	if (fcbcc.is_open())
		cout << "open ciphertext file successfully" << endl;

	ofstream fcbcm(dst_path, ios::out);
	if (fcbcm.is_open())
		cout << "open message file successfully" << endl;

	fk.open(key_path, ios::in);
	if (fk.is_open())
		cout << "open key file successfully" << endl;
	cnt = 0;
	while (cnt < 16)
	{
		fk.unsetf(ios::skipws);
		char temp;
		fk.get(temp);
		seed[cnt] = (char)temp;
		cnt++;
	}


	ifstream fiv(iv_path, ios::in);
	string iv;
	cnt = 0;
	while (cnt < 16)
	{
		fiv.unsetf(ios::skipws);
		char temp;
		fiv.get(temp);
		iv.push_back((char)temp);
		cnt++;
	}
	setIV(iv);

	vector<char> ciphertextCBC;
	int ct = 0;
	int l = filesize(src_path);
	while (ct<l)
	{
		ct++;
 		fcbcc.unsetf(ios::skipws);
		char temp;
		fcbcc.get(temp);
		ciphertextCBC.push_back((char)temp);
		if (ct == 96)
			cout<<endl;
		int a = 1;
	}

	vector<char> messageCBC = CBCDecryption(ciphertextCBC, seed);
	for (auto ch : messageCBC)
		fcbcm << ch;


	cout << endl;
	cout << "done" << endl;
}

int menu()
{
	bool isEnd = 0;
	cout << "|";
	cout << setw(20) << setfill('-');
	cout << "|" << endl;

	cout << setfill(' ');
	cout << "|";
	cout << setw(14) << setiosflags(ios::right) << "function";
	cout << setw(6) << setfill(' ');
	cout << "|" << endl;

	cout << setiosflags(ios::left);
	cout << "|1.AES test" << setw(10) << '|' << endl;
	cout << "|2.CBC test" << setw(10) << '|' << endl;
	cout << "|3.AES encryption" << setw(4) << '|' << endl;
	cout << "|4.AES decryption" << setw(4) << '|' << endl;
	cout << "|5.CBC encryption" << setw(4) << '|' << endl;
	cout << "|6.CBC decryption" << setw(4) << '|' << endl;
	cout << "|7.exit" << setw(14) << '|' << endl;

	cout << "|";
	cout << setw(20) << setfill('-');
	cout << "|" << endl;
	
	cout << endl;
	cout << "choose: " ;
	int choice;
	cin >> choice;
	cout << endl;
	string src_path, key_path, dst_path, iv_path;
	if (choice == 3 || choice == 5)
	{
		src_path = getPath("message");
		key_path = getPath("private key");
		dst_path = getPath("ciphertext");
	}
	if (choice == 4 || choice == 6)
	{
		src_path = getPath("ciphertext");
		key_path = getPath("private key");
		dst_path = getPath("message");
	}
	if(choice==6)
		iv_path = getPath("initial IV");
	
	if (choice == 1)
		AEStest();
	else if (choice == 2)
		CBCtest();
	else if (choice == 3)
		AESEncrption(src_path, key_path, dst_path);
	else if (choice == 4)
		AESDecrption(src_path, key_path, dst_path);
	else if (choice == 5)
		CBCEncryption(src_path, key_path, dst_path);
	else if (choice == 6)
		CBCDecryption(src_path, key_path, dst_path, iv_path);
	else if (choice == 7)
		isEnd = 1;

	cout << "\n\n\n\n\n\n\n";
	cout << "return menu(1) or exit(0)" << endl;
	bool isRet;
	cin >> isRet;
	if (isRet)
		system("cls");
	else
		isEnd = 1;
	return isEnd;
}

int main()
{
	while (!menu());

	return 0;
}
