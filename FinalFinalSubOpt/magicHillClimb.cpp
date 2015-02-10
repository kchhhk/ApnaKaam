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

#ifndef MAGICHILLCLIMB_CPP
	#define MAGICHILLCLIMB_CPP

#include "Problem.h"

int Problem::magicHillClimb(vector<string>& vs, int start, int end)
{
	vector<int> best(3);		// store coordinates and whether left(0) or right(1) neighbour is best
	int bestChangeYet;			// the more negative, the better 
	int loopCount = 0; 			// book-keeping
	int zeroCount = 0;			// if plateau, then for cutoff (initally 100)
	int l = vs[start].length();

	dashCostMatrix = vector<vector<vector<int> > >(noOfStrings , vector<vector<int> >(l, vector<int>(2,numeric_limits<int>::max())));

	initialiseHillClimb(vs,l,start,end);
	
	// for (int i = 0 ; i<noOfStrings ; i++)
	// 	cout << vs[i] << endl;
	//
	// for (int i = 0 ; i<noOfStrings ; i++)
	// {
	// 	for (int j = 0 ; j<l ; j++)
	// 		cout << setw(1) << "(" << dashCostMatrix[i][j][0] <<","<< dashCostMatrix[i][j][1] << ") " ;
	//
	// 	cout <<endl;
	// }
	//  cout << "----------" << endl;
	// 	 for (set<vector<int> >::iterator it = dashSet.begin() ; it!=dashSet.end() ; it++)
	// 		 cout << (*it)[0]<<","<<(*it)[1]<<","<<(*it)[2] <<" ";

	while (true)
	{
		loopCount++;		
		bestChangeYet = 1;			// if doesn't change, then stays positive		
		best = *(dashSet.begin());
		int curBestCost = dashCostMatrix[best[0]][best[1]][best[2]];
		
		if (curBestCost > 0 or zeroCount==25) break;
		
		// for plateau
		if (curBestCost == 0)
		{
			vector<vector<int> > zeroStateIndices;		//[i][j][k] of all zero cost dash movements (k for left or right)
			
			//for all the zero wale neighbours
			for (sIt it = dashSet.begin() ; it!= dashSet.end() ; it++) //iterator syntax
			{
				if (dashCostMatrix[(*it)[0]][(*it)[1]][(*it)[2]] == 0)
					zeroStateIndices.push_back(*it);
				else break;
			}
			
			zeroCount++;	

			int decide = rand() % zeroStateIndices.size();

			best = zeroStateIndices[decide];
		}
		///// ^ for plateau /////
		
		else zeroCount = 0;			// implies crossed plateau
		bool leftSwap = true;		// to check what swapping has been done
		
		performSwap(vs,l,best[0],best[1],best[2],leftSwap);
		
		////////////////////////////////    updating dash costs in log(Nk) time {hopefully}   ///////////////////////
		
		int effX;									// effective X component {3rd out of potentially 4 columns}						
		if (leftSwap == true) effX = best[1];		// | | |i| |  
		else effX = best[1]+1;						// | |i| | |  --> |.| |effX|.|		{ . => may or may not exist}
		
		if (effX > 1)
			for (int i = start ; i < end ; i++)
				if (vs[i][effX-2] == '-')
				{							
					vector<int> item = {i, effX-2 , 1};
					set<vector<int> >::iterator it = returnDashSetItr(item);
					
					if (it != dashSet.end())			// if doesn't exist, dashCostMatrix has infinite value, so goes down tree quickly
					{
						dashSet.erase(it);
					
						int finCost = columnCost(vs, i, effX-2, effX-1, start, end) + columnCost(vs, i, effX-1, effX-2, start, end);
						int curCost = columnCost(vs, i, effX-2, effX-2, start, end) + columnCost(vs, i, effX-1, effX-1, start, end);
			
						dashCostMatrix[i][effX-2][1] = finCost - curCost;
			
						item = {i, effX-2 , 1};
						dashSet.insert(item);
					}
				}
					
				
		if (effX < (l-1))			// 4th column exists
			for (int i = start ; i < end ; i++)
				if (vs[i][effX+1] == '-')
				{
					vector<int> item = {i, effX+1 , 0};
					set<vector<int> >::iterator it = returnDashSetItr(item);
					
					if (it != dashSet.end())
					{
						dashSet.erase(it);
					
						int finCost = columnCost(vs, i, effX+1, effX, start, end) + columnCost(vs, i, effX, effX+1, start, end);
						int curCost = columnCost(vs, i, effX, effX, start, end) + columnCost(vs, i, effX+1, effX+1, start, end);
					
						dashCostMatrix[i][effX+1][0] = finCost - curCost;
					
						item = {i, effX+1 , 0};
						dashSet.insert(item);
					}
				}
				
		// for both effX and effX-1 , need to update left and right move cost
		for (int i = start ; i < end ; i++)
			for (int j = effX-1 ; j<=effX ; j++)	
				if (vs[i][j] == '-')
				{
					if (!(j==effX and effX==(l-1)))
					{
						vector<int> right = {i,j,1};
						set<vector<int> >::iterator itr = returnDashSetItr(right); 
						
						if (itr != dashSet.end())
						{
							dashSet.erase(itr);
						
							int curRightCost = columnCost(vs, i, j, j, start, end) + columnCost(vs, i, j+1, j+1, start, end);
							int finRightCost = columnCost(vs, i ,j, j+1, start, end) + columnCost(vs, i, j+1, j, start, end);
							dashCostMatrix[i][j][1] = finRightCost - curRightCost ;
						
							right = {i,j,1} ;
							dashSet.insert(right); 
						}
					}
					
					if (!(j==effX-1 and j==0))
					{
						vector<int> left  = {i,j,0};
						set<vector<int> >::iterator itl = returnDashSetItr(left);
						
						if (itl != dashSet.end())
						{
							dashSet.erase(itl); 

							int curLeftCost = columnCost(vs, i, j, j, start, end) + columnCost(vs, i, j-1, j-1, start, end);
							int finLeftCost = columnCost(vs, i ,j, j-1, start, end) + columnCost(vs, i, j-1, j, start, end); 										
							dashCostMatrix[i][j][0] = finLeftCost - curLeftCost ;
					
							left = {i,j,0} ; 
							dashSet.insert(left) ; 
						}
					}
				}

	//////////////////////////////////    ^   updated dashes      //////////////////////
	
	}
	
	// checking if column of dashes align, remove if they do
	bool check1 = true;
	vector<bool> isColumnOfDashes(l,false);
	int colOfDashes = 0;
	
	for (int q = 0 ; q<l ; q++)
		if (vs[start][q]=='-')
		{
			check1 = true;
			
			for (int m = start+1 ; m < end ; m++)
				{
					if(vs[m][q]!='-')
						{
							check1 = false;
							break;
						}
				}
				
			if (check1 == true)
			{
				// for (int i = 0 ; i<noOfStrings ; i++)
				// 	cout << vs[i] << "\n";
				// check1=false;
				// cout<<" found you!! "<< l << endl;
				// break;
				
				isColumnOfDashes[q] = true;
				colOfDashes ++ ;
			}
		}
		
	if (colOfDashes!=0)
	{
		vector<string> dashRemovedStringVector(noOfStrings);	
		
		// for (int i = start ; i<end ; i++)
		// 	cout << vs[i] << endl;
		// cout << endl;
			
		for (int i = 0 ; i<l ; i++)
			if (!isColumnOfDashes[i])
				for (int j = start ;  j<end ; j++)
					dashRemovedStringVector[j].push_back(vs[j][i]);
	
		for (int i = start ; i<end ; i++)
				vs[i] = dashRemovedStringVector[i];

		// cout << "deleted you : " << vs[start].length() << endl;
		//
		// for (int i = start ; i<end ; i++)
		// 	cout << vs[i] << endl;
		// cout << endl;
	}
	
	dashSet.clear();			// make ready for next iteration
	return vs[start].length();
}

#endif
