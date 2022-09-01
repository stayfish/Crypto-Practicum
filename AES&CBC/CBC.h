#ifndef CBC
#define CBC
#include<iostream>
#include<cstdlib>
#include<time.h>
#include"AES.h"

vector<char> CBCEncryption(vector<char>, Key);
vector<char> CBCDecryption(vector<char>, Key);
void setIV(string);
string getIV();
#endif 
