#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{

	StringKernel Maria("Maria RAHAB");

	Maria.Print();
	cout << endl;


	cout << (short)Maria.SearchNecklaceFromLeft("RAHABB");
	cout << endl;

	return 0;
}