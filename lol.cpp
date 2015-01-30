#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>

using namespace std;

int main()
{
			
	bitset<5> a = bitset<5>(13);
	bitset<5> b = bitset<5>(12);
	cout <<a <<endl;
	cout << b <<endl;
	bitset<5> c = a^b;
	cout << c <<endl;
}