#ifndef UTIL
#define UTIL
#include<iostream>
#include<iomanip>
#include"F256.h"
#include"const.h"

typedef char Key[16];
typedef char State[4][4];
typedef char SBOX(char);

//vector<char> RotWord(vector<char>);
//vector<char> SubWord(vector<char>);
//vector<Key> KeyExpansion(Key);
//void StrToState(State&,vector<char>);
//vector<char> StateToStr(State);
//void AddRoundKey(State&, Key);
//void SubBytes(State&);
//void ShiftRows(State&);
//void MixColumns(State&);
//
//void InvShiftRows(State&);
//void InvSubBytes(State&);
//void InvMixColumns(State&);

void InvTest(vector<char>, Key);
vector<char> AES(vector<char>, Key);
vector<char> AESInv(vector<char>, Key);

#endif