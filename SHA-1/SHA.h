#ifndef SHA
#define SHA
#include"ulit.h"
#include<fstream>
using namespace std;
bitset<160> SHA1(ifstream& );
string bitset2char(bitset<160>);
#ifdef TEST
string readfile(ifstream& f);
#endif // test

#endif