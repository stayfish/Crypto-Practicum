#ifndef F256
#define F256
#include<vector>
using namespace std;

typedef vector<int> Field;

const Field base = { 1,1,0,1,1,0,0,0 };//1+x+0x2+x3+x4+0x5+0x6+0x7+x8
Field FieldInit();
Field BinaryToField(char);
Field FieldInv(Field);
Field FieldMul(Field,Field);
Field FieldAdd(Field,Field);
Field FieldShift(Field, int, bool isBase=false);
Field* FieldDiv(Field, Field, bool isBase=false);
int deg(Field);
char FieldToBinary(Field);

#endif
