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

#ifndef MAGICLOCALSEARCH_CPP
	#define MAGICLOCALSEARCH_CPP

#include "Problem.h"

void Problem::magicLocalSearch(int time)
{
	int minDash = 0;
	
	for (int i = 0; i<noOfStrings ; i++)
	 	minDash += maxStrLength - strLengths[i];
	
	int minDashCost = minDash*CC;
	
	int bestCostYet = firstEst();
	vector<string> bestStateYet = {"first ","is","best","pls","get","lost"};
	
	for (int loopCount = 0 ; loopCount<5; loopCount++)
	{
		if (loopCount%1 == 0 and loopCount>0) cout << "loop count : " <<loopCount << endl;
		
		vector<bool> stringPlaced(noOfStrings,false);
		vector<int> stringPermutation(noOfStrings);
		int j = 0;
		
		while (j<noOfStrings)
		{
			int temp = rand() % noOfStrings;
			
			if (stringPlaced[temp]==false)
			{
				stringPlaced[temp] = true;
				stringPermutation[j] = temp;
				j++;
			}
		}
		
		vector<string> cur(noOfStrings);
		
		for (int i = 0 ; i<noOfStrings ; i++)
			cur[i] = strings[stringPermutation[i]];
		
		// for (int i = 0 ; i<noOfStrings ; i++)
		// 	cout << cur[i] << endl;
		
		magic(cur, stringPermutation, 0, noOfStrings, 100);
			
		int curCost = allMatchingCost(cur)+ minDashCost + (cur[0].length()-maxStrLength)*CC*noOfStrings;
		
		if (curCost < bestCostYet)
		{
			bestStateYet = cur;
			bestCostYet  = curCost;
			cout << "Best cost yet : " << bestCostYet << " (" << bestStateYet[0].length() << ")" <<endl;
		}	
	}
	
	for (int i = 0 ; i<noOfStrings ; i++)
		cout << bestStateYet[i] << endl;
	
	cout << bestCostYet << endl;
}

#endif
