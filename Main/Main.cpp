#include <iostream>

#include "StringKernel.h"

using namespace std;

int main()
{

	StringKernel Abdou("TTTTTTTTTTTTLML");

	cout << (signed short) Abdou.SearchSheepMasslyFromLeft(0, 14, "TTL", 0, 2) << endl;


}