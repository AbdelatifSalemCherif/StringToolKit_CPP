#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{

	StringKernel Abdou("Abdelatif SALEM CHERIF"), Maria("Maria RAHAB");

	cout << "Abdou : " << Abdou.Value() << endl;
	cout << "Maria : " << Maria.Value() << endl;

	Abdou.Swap(Maria);

	cout << "Abdou : " << Abdou.Value() << endl;
	cout << "Maria : " << Maria.Value() << endl;

	return 0;
}