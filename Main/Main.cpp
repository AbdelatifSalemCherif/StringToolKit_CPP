#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{

	StringKernel Abdou("Abdelatif SALEM CHERIF");

	cout << "Abdou = " << Abdou.GetValue() << endl << endl;

	Abdou.Replace(1400, 200, "Abdou");

	cout << "Abdou = " << Abdou.GetValue() << endl << endl;

	return 0;
}