#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <set>

using namespace std;

// what if first est is soln?


#include "Problem.h"
#include "Problem.cpp"
#include "recurHillClimb.cpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<vector<vector<int> > > Problem::dashCostMatrix;
multiset<vector<int>, compare > Problem::dashSet;

int main()
{
	//std::bitset<4>(10);
	srand (time(0));			// something for randomness
	
	//-------------------------------		INPUT		-------------------------------//
	
	double timeInMin;
	int sizeOfVocab, noOfStrings, CC;
	
	scanf("%lf",&timeInMin);
	scanf("%i",&sizeOfVocab);	
	
	vector<char> vocab;
	string temp;
	getline(cin,temp);		//to read endline
	getline(cin,temp);
	
	for (string::iterator it = temp.begin() ; it!=temp.end() ; it++)
	{
		if ((char)(*it)!=',' and (char)(*it)!=' ')
			vocab.push_back((char)(*it));
	}
	
	scanf("%i",&noOfStrings);	
	
	vector<string> strings;
	getline(cin,temp);		//to read endline
	
	for (int i = 0 ; i<noOfStrings ; i++)
	{
		getline(cin,temp);
		strings.push_back(temp);
	}

	scanf("%i",&CC);	
	
	vector<vector<int> > MC(sizeOfVocab+1 , vector<int>(sizeOfVocab+1));
	
	for (int i = 0 ; i<sizeOfVocab+1 ; i++)
	{		
		for (int j = 0 ; j<sizeOfVocab+1 ; j++)
		{
			int tem;
			scanf("%i", &tem);
			MC[i][j] = tem;
		}
	}
	
	char end;
	scanf("%c",&end);		//reads endline char
	scanf("%c",&end); 
	
//-------------------------------------------------------------------------------//
	clock_t start;
	double diff;
	start = clock();
//	if (end == '#')

	Problem* current = new Problem(sizeOfVocab,noOfStrings,CC,vocab,strings,MC);
	cout << "First est : " << current->firstEst() <<endl;
	
	current->localSearch(5);
	
	diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
	cout<<"time taken: "<< diff <<" sec\n" <<"\n";
	
}