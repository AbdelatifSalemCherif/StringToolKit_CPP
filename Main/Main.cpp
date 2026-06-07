#include <iostream>

#include "StringKernel.h"

using namespace std;

unsigned short _GetNewCapacity(unsigned short NewLength) 
{

	return (NewLength + 15) & ~15;
}

int main()
{

	cout << _GetNewCapacity(0);

	

	


}