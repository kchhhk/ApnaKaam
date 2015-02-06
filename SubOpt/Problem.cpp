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

#ifndef PROBLEM_CPP
	#define PROBLEM_CPP	

#include "Problem.h"

using namespace std;

Problem::Problem(int sOfV, int Ks, int CCo, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos)
{
	charIndex     = vector<int>(256,-1);		//256 for each ASCII, initialised to -1 {error detection}
	minAlphabetMC = vector<int>(sOfV+1);
	
	sizeOfVocab    = sOfV;
	noOfStrings    = Ks;
	CC             = CCo;
	vocab          = V;
	strings        = str;
	MC             = MatCos;
	maxStrLength   = 0;
	DPsolvedMatrix = vector<vector<vector<vector<int> > > >(noOfStrings,vector<vector<vector<int> > >(noOfStrings, vector<vector<int> >()));

	// char Index initialization
	for (int i = 0 ; i <sizeOfVocab ; i++)
		charIndex[(int)vocab[i]] = i;
	
	charIndex[(int)('-')] = sizeOfVocab;		// !
	
	// strLengths and maxStrLength initialization
	strLengths = vector<int>(noOfStrings);
	
	for (int i = 0 ; i<noOfStrings ; i++)
	{
		strLengths[i] = strings[i].length();
		
		if (strLengths[i]>maxStrLength)
			maxStrLength = strLengths[i];
	}
	
	// minAlphabetMC initialization
	for (int i = 0 ; i<=sizeOfVocab; i++)		// <= because dash in the end
	{
		int curmin;
		if (i!=sizeOfVocab) curmin = MC[i][i+1];
		else curmin = MC[i][i-1];
			
		for (int j = 0 ; j<=sizeOfVocab ; j++)
		{
			if (j!=i)
				if (MC[i][j] < curmin) curmin = MC[i][j];
		}
		
		minAlphabetMC[i] = curmin;
	}
	
	//hillClimb(vector<string>{"-agcgcg--a--cg-tt-ag-ttagccagta-cg-cag-atatc","agt--tggca-ttacg-accca-tag-tta-c--atgcggg---","--attt-gcca-aattttgaccaga--ac-gat-catg-ca--a"},44);
	
	/*
	for (int i = 0 ; i<noOfStrings-1 ; i++)
		for (int j = i+1 ; j<noOfStrings ; j++)
				DPsolvedMatrix[i][j] = DPsolve(strings[i],strings[j]);

	// startnode and goalnode initialization	*/
}

/*
const int Problem::DPhst(const Node& node) const
{
	int dashes = 0;				// for CC cost, total min number of dashes to be introduced
	int maxLenRem = 0;			// longest remaining string length
	int h = 0;					// heuristic value
	
	for (int i = 0 ; i<noOfStrings ; i++)
		if (strLengths[i]-node.stateIndices()[i] > maxLenRem)
			maxLenRem = strLengths[i]-node.stateIndices()[i];
	
	for (int i = 0 ; i<noOfStrings ; i++)
		dashes += maxLenRem - (strLengths[i]-node.stateIndices()[i]);
	
	h += CC*dashes;

	for (int i = 0 ; i<noOfStrings-1 ; i++)                         						
		for (int j = i+1 ; j<noOfStrings ; j++)	
			h += DPsolvedMatrix[i][j][node.stateIndices()[i]][node.stateIndices()[j]];

	return h;
}	*/

const int Problem::getCost(char a, char b) const
	{ 
		return MC[charIndex[(int)a]][charIndex[(int)b]];	
	}

const int Problem::firstEst()																		// 	try doing it heuristically
{
	vector<string> tempStrings = strings;
	int firstEstimate = 0;

	for (int i=0; i<noOfStrings; i++)														//	substitute for k by no of strings
	{
		if (strings[i].length() < maxStrLength)													//	substitute for vec by vector of strings
		{
			int noOfDashes = maxStrLength - strings[i].length();
			firstEstimate += CC * noOfDashes;

			for (int j=0; j<noOfDashes ; j++)
				tempStrings[i] += "-" ;
		} 
	}

	for (int i=0; i<noOfStrings-1; i++)
	{
		for (int j=i+1; j<noOfStrings; j++)
		{
			for (int k=0; k<maxStrLength; k++)
			{ 
				firstEstimate += getCost(tempStrings[i][k],tempStrings[j][k]);							
				//strings[i][k] and strings[j][k] matrix lookup
			}
		}
	}

	return firstEstimate;
}

vector<vector<int> > Problem::DPsolve(const string& s1, const string& s2)				// [i][j] has (best soln - CC cost)
{
	vector<vector<int> > nodeCost(s1.length()+1 , vector<int>(s2.length()+1));			// initially includes cost of dashes, remove before returning
	vector<vector<int> > nodeDashes(s1.length()+1 , vector<int>(s2.length()+1));		// no of dashes in optimal solution, need to remove cost in the end
	
	int CCeff = 0;
	int x = s1.length();	   	//	-------> y
	int y = s2.length();		//  |
	
	nodeCost[x][y]   = 0;
	nodeDashes[x][y] = 0;
	x--;
	
	while (x>=0)
	{
		nodeDashes[x][y] = nodeDashes[x+1][y] + 1;
		nodeCost[x][y] = nodeCost[x+1][y] + MC[charIndex[(int)s1[x]]][sizeOfVocab] + CCeff;		// matching with dash
		x--;
	}
	
	x = s1.length(); y--; 
	
	while (y>=0)
	{
		nodeDashes[x][y] = nodeDashes[x][y+1] + 1;
		nodeCost[x][y] = nodeCost[x][y+1] + MC[charIndex[(int)s2[y]]][sizeOfVocab] + CCeff;		// matching with dash
		y--;
	}

	y = s2.length()-1 ; x--;	// x = s1.length() - 1
	
	while (x>=0)
	{
		while (y>=0)
		{
			int botNbr    = nodeCost[x+1][y] + CCeff + MC[sizeOfVocab][charIndex[(int)s1[x]]];			//edge costs
			int diagNbr   = nodeCost[x+1][y+1] + MC[charIndex[(int)s2[y]]][charIndex[(int)s1[x]]];
			int rightNbr  = nodeCost[x][y+1] + CCeff + MC[sizeOfVocab][charIndex[(int)s2[y]]];		
		
			int minNgb = min(botNbr,min(diagNbr , rightNbr));
		
			if (minNgb == botNbr)
			{
				nodeDashes[x][y] = nodeDashes[x+1][y] + 1;
				nodeCost[x][y] = botNbr;
			}
		
			else if (minNgb == diagNbr)
			{
				nodeDashes[x][y] = nodeDashes[x+1][y+1];
				nodeCost[x][y] = diagNbr;		
			}
		
			else // == rightNbr
			{
				nodeDashes[x][y] = nodeDashes[x][y+1] + 1;
				nodeCost[x][y] = rightNbr;		
			}
			
			y--;
		}
		
		x--;
		y = s2.length()-1;
	}
	
	for (int i = 0 ; i<s1.length()+1 ; i++)
		for (int j = 0 ; j <s2.length()+1 ; j++)
			nodeCost[i][j] -= CCeff*nodeDashes[i][j];
			
	return nodeCost;
}

void Problem::printProblemDetails()
{
	cout << "=========================== \n" ;
	cout << "CC             : " << CC << "\n";
	cout << "noOfStrings    : " << noOfStrings << "\n";	
	cout << "size of vocab  : " << sizeOfVocab << "\n";
	cout << "vocab          : {";
	
	for (vector<char>::iterator it = vocab.begin() ; it!=vocab.end() ; it++ )
		cout << " " << *it << "(" << charIndex[(int)(*it)] << ")" << " " ;
	
	cout << "} \n" ; 
	cout << "strings        : ";

	for (int i = 0 ; i<noOfStrings ; i++ )
		cout << strings[i] << "[" << strLengths[i] << "]" << " " ;
	
	cout << "\n";
	cout << "MC matrix \n--------------------------- \n" ;
	
	for (vector<vector<int> >::iterator it = MC.begin() ; it!=MC.end() ; it++ )
	{
		for (vector<int>::iterator itr = it->begin() ; itr!= it->end() ; itr++ )
			cout << *itr << " ";
		
		cout << "\n";
	}
	cout << "MC matrix \n--------------------------- \n" ;
	cout << "min Alphabet wise MC  : ";
	for (vector<int>::iterator it = minAlphabetMC.begin() ; it!=minAlphabetMC.end() ; it++)
		cout << *it << " ";
	
	cout << "\n";
	
	cout << "=========================== \n" ;
	
	//cout << matchingCost(strings[0],strings[1]) << " " << matchingCost(strings[2],strings[1]) << " " << matchingCost(strings[0],strings[2]) <<endl;
}

const int Problem::matchingCost(const string& s1, const string& s2)		//assumes lengths are same
{
	int sum = 0;
	
	for (int i = 0 ; i<s1.length() ; i++)
		sum += MC[charIndex[(int)(s1.at(i))]][charIndex[(int)(s2.at(i))]];
	
	return sum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<string> Problem::randomStateGenerator(int size)
{
	vector<string> state(noOfStrings,string(size,'&'));			// initialised strings with &

	for (int i = 0 ; i<noOfStrings ; i++)
	{
		vector<bool> dashAt(size,false);		// stores if dash has been introduced
		int j = 0;
		
		while (j<(size-strLengths[i]))			// introduced dashes
		{
			int cur = rand() % size;

			if (dashAt[cur]==false)
			{
				dashAt[cur] = true;
				state[i][cur] = '-';
				j++;
			}
		}
		
		int k = 0; int l = 0;
			
		while (k<strLengths[i])
		{
			if (state[i][l]=='&')
			{
				state[i][l] = strings[i][k];
				k++;
			}
			
			l++;
		}
	}
	
//	for (int i = 0 ; i<noOfStrings ; i++)
//		cout << state[i] <<endl;
	
	return state;
}

const int Problem::columnCost(const vector<string>& vs , int i , int j, int col)
{
	int cost =  0;
	
	for (int p = 0 ; p<noOfStrings ; p++)
		if (p!=i)
			cost += MC[charIndex[(int)vs[i][j]]][charIndex[(int)vs[p][col]]];
				
	return cost;
}

const int Problem::allMatchingCost(const vector<string>& vs)
{
	int cost = 0;
	
	for (int i = 0 ; i<noOfStrings-1 ; i++)
		for (int j = i+1 ; j<noOfStrings ; j++)
			cost += matchingCost(vs[i],vs[j]);
	
	return cost;
}

vector<string> Problem::hillClimb(vector<string> vs, int l)			// all strings built up to length l
{
	/* STRATEGY
	 * -------------------------------------------------
	 * Scan all dashes and see if moving left or right
	 * improves the cost at every step
	 * If it does, move to neighbour, else terminate */
	
	vector<int> bestNgb(3);		// store coordinates and whether left(0) or right(1) neighbour is best
	int bestChangeYet;			// the more negative, the better 
	int loopCount = 0; 			// book-keeping
	
	int zeroCount = 0;			// if plateau, then for cutoff
	
//	cout << "Initial matching cost : " << allMatchingCost(vs) << endl;
	
	// primitive now, initialise positions of dashes to look up in constant time
	// modify only updated dashes, use a priority queue
	
	while (true)
	{
		loopCount++;
		
		bestChangeYet = 1;			// if doesn't change, then stays positive
		
		for (int i = 0 ; i<noOfStrings ; i++)
			for (int j = 0 ; j<l ; j++)
				if (vs[i][j] == '-')
					{
						if (j>0 and vs[i][j-1]!='-')
						{
							int curCost = columnCost(vs,i,j,j)   + columnCost(vs,i,j-1,j-1);
							int finCost = columnCost(vs,i,j,j-1) + columnCost(vs,i,j-1,j);
							
							if (finCost - curCost < bestChangeYet)
							{
								bestChangeYet = finCost - curCost;
								bestNgb[0] = i ; bestNgb[1] = j ; bestNgb[2] = 0;
							}
						}
						
						if (j<l-1 and vs[i][j+1]!='-')
						{
							int curCost = columnCost(vs,i,j,j)   + columnCost(vs,i,j+1,j+1);
							int finCost = columnCost(vs,i,j,j+1) + columnCost(vs,i,j+1,j);
							
							if (finCost - curCost < bestChangeYet)
							{
								bestChangeYet = finCost - curCost;
								bestNgb[0] = i ; bestNgb[1] = j ; bestNgb[2] = 1;
							}
						}
					}
		
		if (bestChangeYet > 0 or zeroCount==100) break;
		
		// for plateau
		if (bestChangeYet == 0)
		{
			vector<vector<int> > zeroStateIndices;		//[i][j][k] of all zero cost dash movements (k for left or right)
			
			for (int i = 0 ; i<noOfStrings ; i++)
				for (int j = 0 ; j<l ; j++)
					if (vs[i][j] == '-')
						{
							if (j>0 and vs[i][j-1]!='-')
							{
								int curCost = columnCost(vs,i,j,j)   + columnCost(vs,i,j-1,j-1);
								int finCost = columnCost(vs,i,j,j-1) + columnCost(vs,i,j-1,j);
							
								if (finCost == curCost)										// zero state index
									zeroStateIndices.push_back(vector<int>{i,j,0});
							}
						
							if (j<l-1 and vs[i][j+1]!='-')
							{
								int curCost = columnCost(vs,i,j,j)   + columnCost(vs,i,j+1,j+1);
								int finCost = columnCost(vs,i,j,j+1) + columnCost(vs,i,j+1,j);
							
								if (finCost == curCost)										// zero state index
									zeroStateIndices.push_back(vector<int>{i,j,1});
							}
						}
			
			zeroCount++;	

			int decide = rand() % zeroStateIndices.size();

			bestNgb = zeroStateIndices[decide];
		}
		///// ^ for plateau
		else zeroCount = 0;			// implies crossed plateau
			
		char temp = vs[bestNgb[0]][bestNgb[1]];
		
		// swapping
		if (bestNgb[2]==0)
		{
			vs[bestNgb[0]][bestNgb[1]] = vs[bestNgb[0]][bestNgb[1]-1];
			vs[bestNgb[0]][bestNgb[1]-1] = temp;
		}
		
		else
		{
			vs[bestNgb[0]][bestNgb[1]] = vs[bestNgb[0]][bestNgb[1]+1];
			vs[bestNgb[0]][bestNgb[1]+1] = temp;
		}
		
		
		
		// checking if column of dashes align
	//	bool check1 = true;
		//bool check2 = true;
		
		/*for (int q = 0 ; q<l ; q++)
			if (vs[0][q]=='-')
			{
				for (int m = 1 ; m < noOfStrings ; m++)
					{
						if(vs[m][q]!='-')
							{
								check1 = false;
								break;
							}
					}
				if (check1 == true)
				{for (int i = 0 ; i<noOfStrings ; i++)
						cout << vs[i] << "\n";
					check1=false;
					cout<<" found you!!"<<endl;
					break;
				}
			}
		*/
	}
	//cout << loopCount << endl;
	//for (int i = 0 ; i<noOfStrings ; i++)
	//	cout << vs[i] << "\n";
	
	//cout << "Final matching cost : " << allMatchingCost(vs) << endl;
	//cout << "Loop cout           : " << loopCount << endl;
	
	return vs;
}

void Problem::localSearch(int time)
{
	int minDash = 0;
	
	for (int i = 0; i<noOfStrings ; i++)
	 	minDash += maxStrLength - strLengths[i];
	
	int minDashCost = minDash*CC;
	
	int bestCostYet = firstEst();
	vector<string> bestStateYet;
	
	for (int loopCount = 0 ; loopCount<5000; loopCount++)
		{if (loopCount%500 == 0) {cout << loopCount <<endl;}
		int decide = rand()%100;
		
		if (decide < 30)
		{
			int size = (rand()%(((12*maxStrLength)/10)-maxStrLength))+maxStrLength;
			
			vector<string> cur = randomStateGenerator(size);
			//	cout << "reset start cost: " << allMatchingCost(cur)+ minDashCost + (size-maxStrLength)*CC*noOfStrings << endl;
			cur = hillClimb(cur,size);
			int curCost = allMatchingCost(cur)+ minDashCost + (size-maxStrLength)*CC*noOfStrings;
			
			if (curCost < bestCostYet)
			{
				bestStateYet = cur;
				bestCostYet  = curCost;
				cout << "Best cost yet : " << bestCostYet << endl;
			}	
		}
		
		else if (decide < 80)
		{
			int size = (rand()%(((15*maxStrLength)/10)-(12*maxStrLength)/10))+((12*maxStrLength)/10);
			
			vector<string> cur = randomStateGenerator(size);
		//				cout << "reset start cost: " << allMatchingCost(cur)+ minDashCost + (size-maxStrLength)*CC*noOfStrings << endl;
			cur = hillClimb(cur,size);
			int curCost = allMatchingCost(cur) + minDashCost + (size-maxStrLength)*CC*noOfStrings;
			
			if (curCost < bestCostYet)
			{
				bestStateYet = cur;
				bestCostYet  = curCost;
				cout << "Best cost yet : " << bestCostYet << endl;				
			}
		}
		
		else
		{
			int size = (rand()%(((17*maxStrLength)/10)-(15*maxStrLength)/10))+((15*maxStrLength)/10);
			
			vector<string> cur = randomStateGenerator(size);
		//				cout << "reset start cost: " << allMatchingCost(cur)+ minDashCost + (size-maxStrLength)*CC*noOfStrings << endl;
			cur = hillClimb(cur,size);
			int curCost = allMatchingCost(cur) + minDashCost + (size-maxStrLength)*CC*noOfStrings;
			
			if (curCost < bestCostYet)
			{
				bestStateYet = cur;
				bestCostYet  = curCost;
				cout << "Best cost yet : " << bestCostYet << endl;				
			}
		}
	}
	
	for (int i = 0 ; i<noOfStrings ; i++)
		cout << bestStateYet[i] << endl;
	
	cout << bestCostYet << endl;
}




#endif
