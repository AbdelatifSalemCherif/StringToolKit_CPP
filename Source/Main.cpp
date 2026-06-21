#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{
	StringKernel Abdou;

	Abdou.Assignment("Abdelatif SALEM CHERIF");

	cout << Abdou.CountAllWords(" ") << endl;


	return 0;
}