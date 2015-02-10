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

#ifndef MAGIC_CPP
	#define MAGIC_CPP

#include "Problem.h"

// do size for informedRandomState
// correct final order


void Problem::blockAlign(vector<string>& cur, const vector<int>& orderArray, int start, int end, int length)
{
	// need to align cur[end-1] with previous
	
	int bestChangeYet;
	int bestIndex;
	int swapWithIndex;
	
	while (true)
	{
		bestChangeYet = 0;

		for (int i = 0 ; i<length ; i++)
			if (cur[end-1][i] == '-')
			{
				int l = i;		// move left
				int r = i;		// move right
				
				while (l>=0 and cur[end-1][l] == '-') l--;
				while (r<length and cur[end-1][r] == '-') r++;
				
				if (l>=0)
				{
					int curCost = columnCost(cur,end-1,i,i,start,end) + columnCost(cur,end-1,l,l,start,end);
					int finCost = columnCost(cur,end-1,i,l,start,end) + columnCost(cur,end-1,l,i,start,end);
					
					if (finCost - curCost < bestChangeYet)
					{
						bestChangeYet = finCost - curCost;
						bestIndex     = i;
						swapWithIndex = l;
					}
				}	
				
				if (r<length)
				{
					int curCost = columnCost(cur,end-1,i,i,start,end) + columnCost(cur,end-1,r,r,start,end);
					int finCost = columnCost(cur,end-1,i,r,start,end) + columnCost(cur,end-1,r,i,start,end);
					
					if (finCost - curCost < bestChangeYet)
					{
						bestChangeYet = finCost - curCost;
						bestIndex     = i;
						swapWithIndex = r;
					}
				}
			}
			
		if (bestChangeYet >= 0) break;
		
		else
		{
			char temp = cur[end-1][bestIndex];
			cur[end-1][bestIndex]     = cur[end-1][swapWithIndex];
			cur[end-1][swapWithIndex] = temp;
		}
	}
}

vector<string> Problem::informedRandomState(vector<string>& cur, const vector<int>&  orderArray, int start, int end, int maxLength)
{
	// start with cur such that start...end-2 has same length
	// cur[end-1] may have different length
	// need to determine new size randomly
	// for start...end-2 only introduce columns of dashes
	
	int size;			// determine a better one!!!!!
	
	size = (rand()%(((13*maxLength)/10)-maxLength))+maxLength;
	vector<string> state(noOfStrings,string(size,'&'));			// initialised strings with &

	vector<bool> dashAt(size,false);		// stores if dash has been introduced
	int j = 0;
	
	while (j<(size-cur[start].length()))			// introduced dashes for start...end-2			(column of dashes)
	{
		int temp = rand() % size;
		
		if (dashAt[temp]==false)
		{
			dashAt[temp] = true;
			
			for (int i = start; i<end-1 ; i++)
				state[i][temp] = '-';
			
			j++;
		}
	}
	
	dashAt = vector<bool>(size,false);
	j = 0;
	
	while (j<(size-cur[end-1].length()))			// introduced dashes for end-1
		{
		int temp = rand() % size;

		if (dashAt[temp]==false)
		{
			dashAt[temp] = true;
			
			state[end-1][temp] = '-';

			j++;
		}
	}

	for (int i = start ; i<end ; i++)
	{
		int k = 0; int l = 0;
			
		while (k<cur[i].length())
		{
			if (state[i][l]=='&')
			{
				state[i][l] = cur[i][k];
				k++;
			}
			
			l++;
		}
	}
	
//	for (int i = 0 ; i<noOfStrings ; i++)
//		cout << state[i] <<endl;
	
	return state;
}

void Problem::fitNewString(vector<string>& vs, const vector<int>& orderArray , int start, int end, int maxLength, int loopCount)
{
	// increase length, with upper bound
	// can be proportional to end-start
	// must be built to maxLength at least

	int bestCostYet;
	int minDashCost;
	vector<string> bestStateYet;
	
	for (int i = start; i<end ; i++)
		minDashCost += (maxLength-vs[i].length())*CC;
	
	// initialization
	// cout << "========================" << endl;
	// cout << "fitting " << end-start << "th string" << endl;
	// for (int i = start ; i<end ; i++)
	// 	cout << vs[i] << endl;
	// cout << "========================" << endl << endl;
	vector<string> cur = informedRandomState(vs,orderArray,start,end,maxLength);	// inserts columns for (start,end-1) and randomizes dashes for string to be fit
																					// doesn't copy strings after end!
	// cout << "=======================" << endl;
	// cout << "randomised state" << endl;
	// for (int i = start ; i<end ; i++)
	// 	cout << cur[i] << endl;
	// cout << "========================" << endl;
	blockAlign(cur,orderArray,start,end,cur[start].length());
	// cout << "    block aglign done" << endl;
	// for (int i = start ; i<end ; i++)
	// 	cout << cur[i] << endl;
	// cout << "====================" << endl << endl;
	int finalLength = magicHillClimb(cur , start, end);  				// does a hill climb on cur, eliminates dashes and returns final length of strings
	// cout << "hill climb done" << endl;
	// for (int i = start ; i<end ; i++)
	// 	cout << cur[i] << endl;
	
	bestCostYet  = allMatchingCost(cur, start, end) + minDashCost + (finalLength-maxLength)*CC*(end-start);
	bestStateYet = cur;
	
	for (int i = 0 ; i<loopCount ; i++)
	{
		vector<string> temp = informedRandomState(vs,orderArray,start,end,maxLength);	// inserts columns for (start,end-1) and randomizes dashes for string to be fit
		blockAlign(temp, orderArray,start,end, temp[start].length());
		int finLength = magicHillClimb(temp , start, end);							// does a hill climb on cur, eliminates dashes and returns final length of strings
		
		int tempCost = allMatchingCost(temp, start, end) + minDashCost + (finLength-maxLength)*CC*(end-start);
		if ( tempCost < bestCostYet)
		{
			bestCostYet  = tempCost;
			bestStateYet = temp; 
		}
	}
	
	for (int i = start; i<end ; i++)
		vs[i] = bestStateYet[i];
}

void Problem::magic(vector<string>& vs, const vector<int>& orderArray , int start, int end, int loopCount)
{
	// INVARIANT 1 :: at the end of the function, all the processed strings have the same lengths	
	
	if (end == start+2)		// base case
	{
		int a = orderArray[start];
		int b = orderArray[start+1];
		pair<string,string> temp;
		
		if (a<b) temp = DPfinalString[a][b];
		else temp = DPfinalString[b][a];
		
		vs[start]   = temp.first;
		vs[start+1] = temp.second;
		
		// INVARIANT 1 HOLDS
		
		// cout << "performed magic on : " << (end-start) << "strings" << endl;
//
// 		for (int i = start ; i<end ; i++)
// 			cout << vs[i] << endl;
		
		return;
	}
	
	else 
	{
		magic(vs,orderArray,start,end-1,loopCount);
	
		int maxLength = max(vs[end-1].length(),vs[end-2].length());

		fitNewString(vs, orderArray, start, end, maxLength, loopCount);			// all lengths are same now

		 cout << "performed magic on : " << (end-start) << "strings" << endl;
		//
		// for (int i = start ; i<end ; i++)
		// 	cout << vs[i] << endl;
		
		return;
	}
}




#endif