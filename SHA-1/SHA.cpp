#include"ulit.h"

WORD K_t[4] = { 
	0x5A827999,
	0x6ED9EBA1,
	0x8F1BBCDC,
	0xCA62C1D6 
};
bitset<512> low(bitset<1024> bit)
{
	bitset<512> res;
	for (int i = 0; i < 512; i++)
		res[i] = bit[i];
	return res;
}
bitset<512> high(bitset<1024> bit)
{
	bitset<512> res;
	for (int i = 0; i < 512; i++)
		res[i] = bit[i+512];
	return res;
}
string readfile(ifstream &f)
{
	//读512bit，即64字节
	string res;
	char cur;
	int count = 0;
	while (1)
	{
		cur = f.get();
		if (f.eof())
			break;
		bitset<8> bit(cur);
		res += bit.to_string();
		count++;
		if (count >= 64)
			break;
	}
	return res;
}
//返回模运算结果，且为正数
template<typename T>
T mod(T a, T b)
{
	if (a >= 0)
		return a % b;
	else
		return b-((-a) % b);
}
//串行全加器
WORD operator +(WORD left, WORD right)
{
	WORD res = WORD();
	int carry = 0;
	for (int i = 0; i < WORD_LEN; i++)
	{
		res[i] = left[i] ^ right[i] ^ carry;
		carry = left[i] & right[i] | left[i] & carry | right[i] & carry;
	}
	return res;
}
WORD K(int t)
{
	int idx = t / 20;
	return K_t[idx];
}
//分组读取，每组最长512bit
bitset<1024> SHA1_PAD(string s,int l)
{
	int d = mod(447 - l, 512);
	bitset<1024> temp(s);
	temp = temp << (1 + d + 64);
	bitset<1024> one;
	one.set(d + 64);
	bitset<1024> l_bit(l);
	bitset<1024> res = temp | one | l_bit;
	return res;
}
//将512bit划分为16个字
vector<WORD> divide(bitset<512> M)
{
	vector<WORD> W(16);
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < WORD_LEN; j++)
			W[i][j] = M[(15-i) * WORD_LEN + j];
	return W;
}
//循环左移
WORD ROTL(WORD W,int s)
{
	return (W << s) | (W >> (WORD_LEN - s));
}
WORD ft(WORD B, WORD C, WORD D,int t)
{
	if (t >= 0 && t <= 19)
		return (B & C) | ((~B) & D);
	else if (t >= 20 && t <= 39)
		return B ^ C ^ D;
	else if (t >= 40 && t <= 59)
		return (B & C) | (B & D) | (C & D);
	else if (t >= 60 && t <= 79)
		return B ^ C ^ D;
	else
		return bitset<WORD_LEN>();
}
bitset<160> SHA1(ifstream &f)
{
	vector<bitset<512>> M;
	string s;
	bitset<1024> x;
	s = readfile(f);
	int l = s.length();
	int L = l;
	//x= SHA1_PAD(s);
	int n = 0;
	while (l / 512)
	{
		bitset<512> x_bit(s);
		M.push_back(x_bit);
		n++;
		s = readfile(f);
		l = s.length();
		L = L + l;
	}
	if (l <= 447)
	{
		x = SHA1_PAD(s,L);
		M.push_back(low(x));
		n++;
	}
	else if(l<512)
	{
		x = SHA1_PAD(s,L);
		M.push_back(high(x));
		M.push_back(low(x));
		n++;
		n++;
	}

	WORD k[80];
	WORD W[80];
	WORD A, B, C, D, E;
	for (int t = 0; t < 80; t++)
		k[t] = K(t);
	WORD H[5] = {
		0x67452301,
		0xEFCDAB89,
		0x98BADCFE,
		0x10325476,
		0xC3D2E1F0
	};
	for (int i = 0; i < n; i++)
	{
		vector<WORD> W(80);
		vector<WORD> W1 = divide(M[i]);
		for (int i = 0; i < 16; i++)
			W[i] = W1[i];
		for (int t = 16; t < 80; t++)
			W[t] = ROTL(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		A = H[0];
		B = H[1];
		C = H[2];
		D = H[3];
		E = H[4];
		for (int t = 0; t < 80; t++)
		{
			WORD temp = ROTL(A, 5) + ft(B, C, D, t) + E + W[t] + k[t];
			E = D;
			D = C;
			C = ROTL(B, 30);
			B = A;
			A = temp;
		}
		H[0] = H[0] + A;
		H[1] = H[1] + B;
		H[2] = H[2] + C;
		H[3] = H[3] + D;
		H[4] = H[4] + E;
	}
	string h[5];
	for (int i = 0; i < 5; i++)
		h[i] = H[i].to_string();
	return bitset<160>(h[0] + h[1] + h[2] + h[3] + h[4]);
}
string bitset2char(bitset<160> bit)
{
	char ch = 0;
	string res;

	for (int i = 19; i >=0; i--)
	{
		ch = 0;
		for (int j = 0; j < 8; j++)
		{
			ch = ch | (bit[i * 8 + j] << j);
		}
		res.push_back(ch);
	}

	return res;
}