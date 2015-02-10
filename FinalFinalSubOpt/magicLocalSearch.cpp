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
#include <string>

#ifndef MAGICLOCALSEARCH_CPP
	#define MAGICLOCALSEARCH_CPP

#include "Problem.h"

clock_t start;

void Problem::magicLocalSearch(int time)
{
	int minDash = 0;
	
	for (int i = 0; i<noOfStrings ; i++)
	 	minDash += maxStrLength - strLengths[i];
	
	int minDashCost = minDash*CC;
	
	// initialization
	int bestCostYet = firstEst();
	vector<string> bestStateYet(noOfStrings);
	
	for (int i = 0 ; i<noOfStrings ; i++)
	{
		bestStateYet[i] = strings[i];

		for (int j = 0 ; j<maxStrLength-strings[i].length() ; j++)
			bestStateYet[i] += '-';
	}
	
	
	for (int loopCount = 0 ; loopCount<5; loopCount++)
	{
		if (loopCount%1 == 0 and loopCount>0) cout << "loop count : " <<loopCount << endl;
		
		vector<bool> stringPlaced(noOfStrings,false);
		vector<int> stringPermutation(noOfStrings);
		int j = 0;
		
		while (j<noOfStrings)						// look for a better way
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
		
		magic(cur, stringPermutation, 0, noOfStrings, 40 + min(60,loopCount*20));
			
		int curCost = allMatchingCost(cur)+ minDashCost + (cur[0].length()-maxStrLength)*CC*noOfStrings;
		
		if (curCost < bestCostYet)
		{
			vector<string> final(noOfStrings);
			
			for (int i = 0 ; i<noOfStrings ; i++)
				final[stringPermutation[i]] = cur[i];

			bestStateYet = final;
			bestCostYet  = curCost;
			cout << "Best cost yet : " << bestCostYet << " (" << bestStateYet[0].length() << ")" <<endl;
		}	
	}
	cout << endl;
	for (int i = 0 ; i<noOfStrings ; i++)
		cout << bestStateYet[i] << endl;
	
	cout << bestCostYet << endl;
}

#endif
