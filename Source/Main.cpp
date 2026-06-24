#include <iostream>

#include "StringKernel.h"

using namespace std;


int main()
{
	string Kamel = "Abdelatif SALEM CHERIF";

	StringKernel Abdou(Kamel.data());

	cout << "Abdou Words : " << Abdou.Data() << endl;


	return 0;
}