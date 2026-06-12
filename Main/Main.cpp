#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{

	StringKernel Abdou("Abdelatif SALEM CHERIF"), Maria("Maria RAHAB");

	cout << "Capacity = " << Abdou.Capacity() << endl;
	cout << "Length = " << Abdou.Length() << endl;
	cout << "Size = " << Abdou.Size() << endl;


	Abdou.Insert(9, " SALlEM");

	cout << "Capacity = " << Abdou.Capacity() << endl;
	cout << "Length = " << Abdou.Length() << endl;
	cout << "Size = " << Abdou.Size() << endl;

	cout << Abdou.Value() << endl;

	return 0;
}