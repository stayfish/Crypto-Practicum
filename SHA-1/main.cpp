#include"SHA.h"
#include<iostream>
#include<iomanip>
#include<stdlib.h>
using namespace std;

#define FAIL_MESSAGE 1
#define FAIL_MAC 2
#define MY_TEST
#ifndef MY_TEST
#define PPT_TEST
string default_src = "message2.txt";
string default_dst = "MAC2.txt";
#else
//string default_src = "message.txt";
//string default_src = "message3.txt";
string default_src = "message4.txt";
string default_dst = "MAC.txt";
#endif // !MY_TEST
#ifdef TEST
void bittest()
{
	string t= "01";
	bitset<2> test(t);
	cout << test[0] << endl;
	int l = 21;
	bitset<5> bit(l);
	cout << "initialize with integer 21," << bit << endl;
	string s = "10010010";
	bitset<7> bit2(s);
	cout << "initialize with string 'aaa'," << bit2 << endl;//只能读01串的前k个
}
void readfiletest(std::ifstream&f)
{
	string res;
	char cur;
	int count = 0;
	while (1)
	{
		cur = f.get();
		if (f.eof())
			break;
		res.push_back(cur);
		count++;
	}
	cout << count << endl;
	cout << res.length() << endl;
	cout << res << endl;

}
#endif
string getPath(string s)
{
	cout << "input the path of " << s << " is:";
	string path;
	cin >> path;
	//default
	if (path == "m")
		path = default_src;
	else if (path == "mac")
		path = default_dst;
	return path;
}
int menu()
{
	bool isEnd = 0;
	cout << "1 for SHA-1 algorithm" << endl;
	cout << "0 for exit" << endl;

	cout << "choose: ";
	int choice;
	cin >> choice;
	cout << endl;
	string src_path, dst_path;
	if (choice == 1)
	{
		src_path = getPath("message");
		dst_path = getPath("mac");
	}

	if (choice == 1)
	{
		bitset<160> mac;
		ifstream f(src_path, ios::in);
		if (f.is_open())
			cout << "open message file successfully" << endl;
		else
		{
			cout << "fail opening message file" << endl;
			exit(FAIL_MESSAGE);
		}
		mac = SHA1(f);
		ofstream MAC(dst_path, ios::out);
		if (MAC.is_open())
			cout << "open MAC file successfully" << endl;
		else
		{
			cout << "file opening MAC file" << endl;
			exit(FAIL_MAC);
		}
		string mac_hexstring = bitset2char(mac);
		for (auto ch : mac_hexstring)
			MAC << hex << setw(2) << setfill('0') << ((unsigned int)ch & 0xff);
	}
	else if (choice == 0)
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
#ifdef TEST
	ifstream f("message.txt", ios::in);
	if (f.is_open())
		cout << "success" << endl;
	bittest();
	readfiletest(f);
#endif // TEST

#ifdef RUN
	while (!menu());
#endif // RUN
	return 0;
}