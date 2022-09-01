#include"F256.h"
int loc = 0;
bool isOne(Field f)
{
	return f[0] & !f[1] & !f[2] & !f[3] & !f[4] & !f[5] & !f[6] & !f[7];
}
bool isZero(Field f)
{
	return !f[0] & !f[1] & !f[2] & !f[3] & !f[4] & !f[5] & !f[6] & !f[7];
}
Field FieldInit()
{
	return vector<int>(8, 0);
}

int deg(Field f)
{
	int res = 0;
	for (int i = 7; i > 0; i--)
	{
		if (f[i] != 0)
		{
			res = i;
			break;
		}
	}
	return res;
}

Field BinaryToField(char s)
{
	Field f = FieldInit();
	for (int i = 0; i < 8; i++)
		f[i] = (s >> i) & 1;
	return f;
}

char FieldToBinary(Field f)
{
	char ch = 0;
	for (int i = 0; i < 8; i++)
		ch += f[i] << i;
	return ch;
}

Field FieldAdd(Field f1,Field f2)
{
	Field res = FieldInit();
	for (int i = 0; i < 8; i++)
		res[i] = f1[i] ^ f2[i];
	return res;
}

Field FieldInv(Field f)
{
	if (isZero(f))
		return f;
	if (isOne(f))
		return f;
	Field *arr;
	//Extended Euclidean algorithm
	Field s0 = FieldInit(), s1 = FieldInit(), s2 = FieldInit();
	Field t0 = FieldInit(), t1 = FieldInit(), t2 = FieldInit();
	Field r0 = base;
	s0[0] = 1;
	Field r1 = f;
	t1[0] = 1;
	Field q = FieldInit();
	Field r2 = base;
	//1. r0=r1*q+r2
	//2. r0<=r1
	//3. r1<=r2
	bool isBase = true;
	while (!isOne(r2))
	{
		arr = FieldDiv(r0, r1,isBase);
		q = arr[0];
		r2 = arr[1];
		s2 = FieldAdd(s0, FieldMul(q, s1));
		t2 = FieldAdd(t0, FieldMul(q, t1));
		isBase = false;
		r0 = r1;
		r1 = r2;
		s0 = s1;
		t0 = t1;
		s1 = s2;
		t1 = t2;
	}
	loc++;
	return t2;
}

// l<8
Field FieldShift(Field f, int n,bool isBase)
{
	Field res = f;
	Field temp;
	if (n == 0)
		return res;
	for (int i = 1; i <= n; i++)
	{
		temp = res;
		for (int j = 0; j < 7; j++)
			res[j + 1] = temp[j];
		res[0] = 0;
		if(temp[7]==1&&(i!=n||!isBase))
			res = FieldAdd(res, base);
	}
	return res;
}

Field FieldMul(Field f1,Field f2)
{
	Field res = FieldInit();
	Field temp;
	for (int i = 0; i < 8; i++)
	{
		if (f2[i])
		{
			temp = FieldShift(f1, i);
			res = FieldAdd(res, temp);
		}
	}
	return res;
}

Field* FieldDiv(Field f1, Field f2, bool isBase)
{
	Field *arr;
	arr = new Field[2];
	arr[0] = FieldInit();
	arr[1] = FieldInit();
	int r_deg, divisor_deg;
	int shift;
	Field divisor = f2;
	Field q = FieldInit();
	Field r = f1;
	r_deg = isBase ? 8 : deg(r);
	divisor_deg = deg(divisor);
	int round = 0;
	while (r_deg >= divisor_deg)
	{
		round++;
		shift = r_deg - divisor_deg;
		q[shift] = 1;
		if(round==1)
			r = FieldAdd(r, FieldShift(divisor, shift,isBase));
		else
			r = FieldAdd(r, FieldShift(divisor, shift));
		r_deg = deg(r);
		divisor_deg = deg(divisor);
	}
	arr[0] = q;
	arr[1] = r;
	return arr;
}

