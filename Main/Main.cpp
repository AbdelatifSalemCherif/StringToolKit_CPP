#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{

	StringKernel Abdou("Abdelatif SALEM CHERIF");

	cout << "Abdou = " << Abdou.Value() << endl << endl;

	Abdou.Replace(0, 8, "Abdou");

	cout << "Abdou = " << Abdou.Value() << endl << endl;

	return 0;
}