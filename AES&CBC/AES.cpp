#include"AES.h"


void AddRoundKey(State&s, Key k)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			int idx = row + col*4;
			s[row][col] = s[row][col] ^ k[idx];
		}
	}
}

char sbox(char state)
{
	Field f = BinaryToField(state);
	f = FieldInv(f);
	char s = FieldToBinary(f);
	int matrix[8][8] = { 1,1,1,1,1,0,0,0,
						 0,1,1,1,1,1,0,0,
						 0,0,1,1,1,1,1,0,
						 0,0,0,1,1,1,1,1,
						 1,0,0,0,1,1,1,1,
						 1,1,0,0,0,1,1,1,
						 1,1,1,0,0,0,1,1,
						 1,1,1,1,0,0,0,1 };
	int y[8] = { 0,0,0,0,0,0,0,0 };
	int c[8] = { 0,1,1,0,0,0,1,1 };
	for (int i = 0; i <8; i++)
	{
		int x[8];
		for (int j = 0; j < 8; j++)
		{
			x[j] = (s >> (7-j)) & 1;
			y[i] = y[i] + x[j] * matrix[i][j];
		}
	}
	for (int i = 0; i < 8; i++)
		y[i] = (y[i] + c[i])%2;

	char res = y[0];
	for (int i = 1; i < 8; i++)
	{
		res = (res << 1) + y[i];
	}
	return res;
}

void SubBytes(State &s)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
			s[row][col] = sbox(s[row][col]);
	}

}

void ShiftRows(State &s)
{
	char temp[4][4];
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			temp[row][col] = s[row][col];
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			s[row][col]=temp[row][(col+row)%4];
}

void MixColumns(State &s)
{
	char matrix0[4][4] = {
		0x2,0x3,0x1,0x1,
		0x1,0x2,0x3,0x1,
		0x1,0x1,0x2,0x3,
		0x3,0x1,0x1,0x2
	};
	Field matrix[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = BinaryToField(matrix0[i][j]);
	Field t[4];
	Field u[4];
	for (int col = 0; col < 4; col++)
	{
		for (int i = 0; i < 4; i++)
			t[i] = BinaryToField(s[i][col]);
		
		for (int row = 0; row < 4; row++)
		{
			u[row] = FieldInit();
			for (int i = 0; i < 4; i++)
				u[row] = FieldAdd(u[row], FieldMul(matrix[row][i], t[i]));
		}
		for (int i = 0; i < 4; i++)
			s[i][col] = FieldToBinary(u[i]);
	}
}

void StrToState(State& s, vector<char> ch)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			s[row][col] = ch[row  + 4*col];
		}
	}

}

vector<char> StateToStr(State s)
{
	vector<char> ch(16);
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			ch[row + col*4] = s[row][col];
		}
	}
	return ch;
}

vector<char> RotWord(vector<char> b)
{
	vector<char> res(4, 0);
	for (int i = 0; i < 4; i++)
		res[i] = b[(i + 1) % 4];
	return res;
}

vector<char> SubWord(vector<char> b)
{
	vector<char> res(4, 0);
	for (int i = 0; i < 4; i++)
		res[i] = sbox(b[i]);
	return res;
}

vector<char> XORWord(vector<char> a, vector<char> b)
{
	vector<char> res(4, 0);
	for (int i = 0; i < 4; i++)
		res[i] = a[i] ^ b[i];
	return res;
}

vector<char> XORWord(vector<char> a, int b)
{
	vector<char> res(4, 0);
	for (int i = 0; i < 4; i++)
	{
		int temp = (b >> 8 * (3 - i)); 
		temp = temp & (0xff);
		res[i] = a[i] ^ temp;
	}
	return res;
}

vector<Key> KeyExpansion(Key seed)
{
	vector<Key> res(Nr + 1);
	vector<vector<char>> w((Nr + 1) * 4, vector<char>(4, 0));
	vector<char> temp(4, 0);
	int RCon[11] = {
		0x00000000,
		0x01000000,
		0x02000000,
		0x04000000,
		0x08000000,
		0x10000000,
		0x20000000,
		0x40000000,
		0x80000000,
		0x1B000000,
		0x36000000
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			w[i][j] = seed[4 * i + j];
	}
	for (int i = 4; i < (Nr + 1) * 4; i++)
	{
		temp = w[i - 1];
		if (i % 4 == 0)
			temp = XORWord(SubWord(RotWord(temp)), RCon[i / 4]);
		w[i] = XORWord(w[i - 4], temp);
	}
	for (int i = 0; i < Nr + 1; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			res[i][j] = w[4 * i + j / 4][j % 4];
		}
	}
	return res;
}

void InvShiftRows(State& s)
{
	char temp[4][4];
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			temp[row][col] = s[row][col];
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			s[row][col] = temp[row][(col - row + 4) % 4];
}

char sboxInv(char state)
{
	int matrix[8][8] = { 0,1,0,1,0,0,1,0,
						 0,0,1,0,1,0,0,1,
						 1,0,0,1,0,1,0,0,
						 0,1,0,0,1,0,1,0,
						 0,0,1,0,0,1,0,1,
						 1,0,0,1,0,0,1,0,
						 0,1,0,0,1,0,0,1,
						 1,0,1,0,0,1,0,0 };

	int y[8];
	int c[8] = { 0,1,1,0,0,0,1,1 };

	for (int j = 0; j < 8; j++)
	{
		y[j] = (state >> (7 - j)) & 1;
	}

	for (int i = 0; i < 8; i++)
		y[i] = (y[i] + c[i]) % 2;

	int x[8] = { 0,0,0,0,0,0,0,0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			x[i] = (x[i] + y[j] * matrix[i][j])%2;
		}
	}
	

	char res = x[0];
	for (int i = 1; i < 8; i++)
	{
		res = (res << 1) + x[i];
	}
	Field f = BinaryToField(res);
	f = FieldInv(f);
	res = FieldToBinary(f);
	return res;
}

void InvSubBytes(State& s)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
			s[row][col] = sboxInv(s[row][col]);
	}
}

void InvMixColumns(State& s)
{
	char matrix0[4][4] = {
		0x0e,0x0b,0x0d,0x09,
		0x09,0x0e,0x0b,0x0d,
		0x0d,0x09,0x0e,0x0b,
		0x0b,0x0d,0x09,0x0e
	};
	Field matrix[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = BinaryToField(matrix0[i][j]);
	Field t[4];
	Field u[4];
	for (int col = 0; col < 4; col++)
	{
		for (int i = 0; i < 4; i++)
			t[i] = BinaryToField(s[i][col]);

		for (int row = 0; row < 4; row++)
		{
			u[row] = FieldInit();
			for (int i = 0; i < 4; i++)
				u[row] = FieldAdd(u[row], FieldMul(matrix[row][i], t[i]));
		}
		for (int i = 0; i < 4; i++)
			s[i][col] = FieldToBinary(u[i]);
	}
}

void PrintState(State s)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << hex << setw(2) << setfill('0') << ((unsigned int)s[i][j] & 0xff) << " ";
		cout << endl;
	}
}

void InvTest(vector<char> code, Key seed)
{
	//inv component test
	//matrix inverse test
	cout << endl;
	cout << "inverst test:" << endl;
	cout << endl;

	cout << "1. InvSubBytes test:" << endl;
	cout << "sbox inverse matrix test:" << endl;
	int matrix[8][8] = { 1,1,1,1,1,0,0,0,
						 0,1,1,1,1,1,0,0,
						 0,0,1,1,1,1,1,0,
						 0,0,0,1,1,1,1,1,
						 1,0,0,0,1,1,1,1,
						 1,1,0,0,0,1,1,1,
						 1,1,1,0,0,0,1,1,
						 1,1,1,1,0,0,0,1 };
	int matrix_inv[8][8] = { 0,1,0,1,0,0,1,0,
							 0,0,1,0,1,0,0,1,
							 1,0,0,1,0,1,0,0,
							 0,1,0,0,1,0,1,0,
							 0,0,1,0,0,1,0,1,
							 1,0,0,1,0,0,1,0,
							 0,1,0,0,1,0,0,1,
							 1,0,1,0,0,1,0,0 };
	int mul[8][8];
	memset(mul, 0, 64 * sizeof(int));
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 8; k++)
				mul[i][j] = (mul[i][j] + matrix[i][k] * matrix_inv[k][j]) % 2;
	cout << "matrix*matrix_inv is:" << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			cout << mul[i][j] << " ";
		cout << endl;
	}
	State s0 = {
		0x19,0xa0,0x9a,0xe9,
		0x3d,0xf4,0xc6,0xf8,
		0xe3,0xe2,0x8d,0x48,
		0xbe,0x2b,0x2a,0x08
	};
	cout << "state s is:" << endl;
	PrintState(s0);
	SubBytes(s0);
	cout << "after SubBytes:" << endl;
	PrintState(s0);
	InvSubBytes(s0);
	cout << "after InvSubBytes:" << endl;
	PrintState(s0);


	cout << "2. InvShiftRows test:" << endl;
	State s1 = {
		0xd4,0xe0,0xb8,0x1e,
		0x27,0xbf,0xb4,0x41,
		0x11,0x98,0x5d,0x52,
		0xae,0xf1,0xe5,0x30
	};
	cout << "state s is:" << endl;
	PrintState(s1);
	ShiftRows(s1);
	cout << "after ShiftRows:" << endl;
	PrintState(s1);
	InvShiftRows(s1);
	cout << "after InvShiftRows:" << endl;
	PrintState(s1);
	cout << endl;


	cout << "3. InvMixColumns test:" << endl;
	State s2 = {
		0xd4,0xe0,0xb8,0x1e,
		0xbf,0xb4,0x41,0x27,
		0x5d,0x52,0x11,0x98,
		0x30,0xae,0xf1,0xe5,
	};
	cout << "state s is:" << endl;
	PrintState(s2);
	MixColumns(s2);
	cout << "after MixColumns:" << endl;
	PrintState(s2);
	InvMixColumns(s2);
	cout << "after InvMixColumns:" << endl;
	PrintState(s2);
	cout << endl;

	//CBC test
	vector<char> message = AESInv(code, seed);
	cout << "message hex is:" << endl;
	for (char ch : message)
		cout << hex << setw(2) << setfill('0') << ((unsigned int)ch & 0xff) << ' ';
	cout << endl;
}


vector<char> AES(vector<char> x, Key seed)
{
	vector<Key> k = KeyExpansion(seed);
	vector<char> res;
	State sta;
	StrToState(sta, x);
	AddRoundKey(sta, k[0]);
	for (int i = 1; i < Nr; i++)
	{
		SubBytes(sta);
		ShiftRows(sta);
		MixColumns(sta);
		AddRoundKey(sta, k[i]);
	}
	SubBytes(sta);
	ShiftRows(sta);
	AddRoundKey(sta, k[Nr]);
	res = StateToStr(sta);

	return res;
}

vector<char> AESInv(vector<char> code, Key seed)
{
	vector<Key> k = KeyExpansion(seed);
	vector<char> res;
	State sta;
	StrToState(sta, code);

	AddRoundKey(sta, k[Nr]);
	InvShiftRows(sta);
	InvSubBytes(sta);

	for (int i = Nr - 1; i > 0; i--)
	{
		AddRoundKey(sta, k[i]);
		InvMixColumns(sta);
		InvShiftRows(sta);
		InvSubBytes(sta);
	}
	AddRoundKey(sta, k[0]);

	res = StateToStr(sta);

	return res;
}