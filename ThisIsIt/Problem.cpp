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
#include <limits>
#include <iomanip>

#ifndef PROBLEM_CPP
	#define PROBLEM_CPP	

#include "Problem.h"

using namespace std;

// need to fix if initial state is best state

Problem::Problem(int sOfV, int Ks, int CCo, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos)
{
	charIndex     = vector<int>(256,-1);		//256 for each ASCII, initialised to -1 {error detection}
	
	sizeOfVocab    = sOfV;
	noOfStrings    = Ks;
	CC             = CCo;
	vocab          = V;
	strings        = str;
	MC             = MatCos;
	maxStrLength   = 0;
	DPfinalString  = vector<vector<pair<string,string> > >(noOfStrings, vector<pair<string,string> >(noOfStrings));
	//DPsolvedMatrix = vector<vector<vector<vector<int> > > >(noOfStrings,vector<vector<vector<int> > >(noOfStrings, vector<vector<int> >()));

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
	

//	hillClimb(vector<string>{"caagtagtcc","g-ata-accg","-cccgat-ta"},10);
	
	/*
	for (int i = 0 ; i<noOfStrings-1 ; i++)
		for (int j = i+1 ; j<noOfStrings ; j++)
				DPsolvedMatrix[i][j] = DPsolve(strings[i],strings[j]);  */
	
	for (int i = 0 ; i<noOfStrings-1 ; i++)
		for (int j = i+1 ; j<noOfStrings ; j++)
		{
			DPfinalString[i][j] = DPsolve(strings[i],strings[j]);
			//cout << DPfinalString[i][j].first << " " << DPfinalString[i][j].first.length() << endl << DPfinalString[i][j].second << " " << DPfinalString[i][j].second.length() << endl;
			//cout << matchingCost(DPfinalString[i][j].first,DPfinalString[i][j].second) << endl<<endl;
		}
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

const int Problem::firstEst()																		
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

pair<string,string> Problem::DPsolve(const string& s1, const string& s2)				// [i][j] has (best soln - CC cost)
{
	vector<vector<int> > nodeCost(s1.length()+1 , vector<int>(s2.length()+1));			// initially includes cost of dashes, remove before returning
	vector<vector<int> > nodeDashes(s1.length()+1 , vector<int>(s2.length()+1));		// no of dashes in optimal solution, need to remove cost in the end
	vector<vector<pair<int,int> > > nextNode(s1.length()+1 , vector<pair<int, int> >(s2.length()+1));			// stores next Node for optimal solution

	int CCeff = CC;				// keeping CC here
	int x = s1.length();	   	//	-------> y
	int y = s2.length();		//  |
	
	nodeCost[x][y]   = 0;
	nodeDashes[x][y] = 0;
	x--;

	while (x>=0)
	{
		nextNode[x][y] = pair<int,int>(x+1,y);
		nodeDashes[x][y] = nodeDashes[x+1][y] + 1;
		nodeCost[x][y] = nodeCost[x+1][y] + MC[charIndex[(int)s1[x]]][sizeOfVocab] + CCeff;		// matching with dash
		x--;
	}
	
	x = s1.length(); y--; 
	
	while (y>=0)
	{
		nextNode[x][y] = pair<int,int>(x,y+1);
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
				nextNode[x][y] = pair<int,int>(x+1,y);
				nodeDashes[x][y] = nodeDashes[x+1][y] + 1;
				nodeCost[x][y] = botNbr;
			}
		
			else if (minNgb == diagNbr)
			{
				nextNode[x][y] = pair<int,int>(x+1,y+1);
				nodeDashes[x][y] = nodeDashes[x+1][y+1];
				nodeCost[x][y] = diagNbr;		
			}
		
			else // == rightNbr
			{
				nextNode[x][y] = pair<int,int>(x,y+1);
				nodeDashes[x][y] = nodeDashes[x][y+1] + 1;
				nodeCost[x][y] = rightNbr;		
			}
			
			y--;
		}
		
		x--;
		y = s2.length()-1;
	}
	
	// for (int i = 0 ; i<s1.length()+1 ; i++)
	// 	for (int j = 0 ; j <s2.length()+1 ; j++)
	// 		nodeCost[i][j] -= CCeff*nodeDashes[i][j];
	
	//generating final strings
	
	int i = 0 ; int j = 0;
	pair<string,string> final;

	while (!(i==s1.length() and j==s2.length()))
	{
		int i_next = nextNode[i][j].first; 
		int j_next = nextNode[i][j].second;
		
		if (i_next==i+1 and j_next==j+1)
		{
			final.first  += s1[i];
			final.second += s2[j];
		}
		
		else if (i_next==i+1 and j_next==j)
		{
			final.first  += s1[i];
			final.second += '-';
		}
		
		else if (i_next==i and j_next==j+1)
		{
			final.first  += '-';
			final.second += s2[j];
		}
		
		i = i_next ; j = j_next;
	}
			
	return final;
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

/////////////////     HILL CLIMB HELPERS     ////////////////

const int Problem::columnCost(const vector<string>& vs , int i , int j, int col)
{
	int cost =  0;
	
	for (int p = 0 ; p<noOfStrings ; p++)
		if (p!=i)
			cost += MC[charIndex[(int)vs[i][j]]][charIndex[(int)vs[p][col]]];
				
	return cost;
}

const int Problem::columnCost(const vector<string>& vs , int i , int j, int col, int start, int end)
{
	int cost =  0;
	
	for (int p = start ; p<end ; p++)
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

const int Problem::allMatchingCost(const vector<string>& vs, int start, int end)
{
	int cost = 0;
	
	for (int i = start ; i<end-1 ; i++)
		for (int j = i+1 ; j<end ; j++)
			cost += matchingCost(vs[i],vs[j]);
	
	return cost;
}

struct compare 
{
	bool operator() (const vector<int>& lhs, const vector<int>&rhs)
	{
		return (Problem::dashCostMatrix[lhs[0]][lhs[1]][lhs[2]] < Problem::dashCostMatrix[rhs[0]][rhs[1]][rhs[2]]); 
	}
};
//////////////////////////////////////////////////////////////

void Problem::initialiseHillClimb(vector<string>& vs, int l, int start, int end)
{
	for (int i = start ; i<end ; i++)
		for (int j = 0 ; j<l ; j++)
			if (vs[i][j] == '-')
			{
				// for left swap {if not possible, then is infty becasue of initialisation}
				if (!(j==0 or vs[i][j-1]=='-')) 		
				{
					int curCost = columnCost(vs,i,j,j,start,end)   + columnCost(vs,i,j-1,j-1,start,end);
					int finCost = columnCost(vs,i,j,j-1,start,end) + columnCost(vs,i,j-1,j,start,end);
					
					dashCostMatrix[i][j][0] = finCost - curCost; //cout << dashCostMatrix[i][j][0] <<endl;
					vector<int> cur = {i,j,0};
					dashSet.insert(cur);
				}
				
				// for right swap {if not possible, then is infty becasue of initialisation}
				if (!(j==l-1 or vs[i][j+1]=='-'))
				{
					int curCost = columnCost(vs,i,j,j,start,end)   + columnCost(vs,i,j+1,j+1,start,end);
					int finCost = columnCost(vs,i,j,j+1,start,end) + columnCost(vs,i,j+1,j,start,end);
					
					dashCostMatrix[i][j][1] = finCost - curCost; //cout << dashCostMatrix[i][j][1] << endl;
					vector<int> cur = {i,j,1};
					dashSet.insert(cur);
				}
			}
}

sIt Problem::returnDashSetItr(vector<int> ijk)			// see typedef for sIt, returns iterator of vec ijk if found, else returns dashSet.end()
{
	pair<sIt,sIt> ret = dashSet.equal_range(ijk);
	
	for (sIt it = ret.first ; it != ret.second ; it++)
	{
		if (((*it)[0]==ijk[0]) and ((*it)[1]==ijk[1]) and ((*it)[2]==ijk[2]))
			return it;
	}
		
	return dashSet.end();
}

void Problem::performSwap(vector<string>& vs, int l, int i, int j, int k, bool& leftSwap)  
// k=0 for left, k=1 for right
{ 
	char temp = vs[i][j];
	int maxInt = numeric_limits<int>::max();
	
	// remove from dashSet for that index
	sIt itl = returnDashSetItr(vector<int>{i,j,0});
	sIt itr = returnDashSetItr(vector<int>{i,j,1});

	if (itl != dashSet.end()) dashSet.erase(itl); 		
	if (itr != dashSet.end()) dashSet.erase(itr); 		
	
	if (k==0)	// swap with left
	{
		vs[i][j] = vs[i][j-1];					// take care of edge cases. remove left/right or add left/right
		vs[i][j-1] = temp;						// just add to dashSet with inf, hillClimb recomputes values
		dashCostMatrix[i][j][0] = maxInt ; dashCostMatrix[i][j][1] = maxInt; // alphabet at place now, must have infty {INVARIANT}
		dashSet.insert(vector<int>{i,j-1,1}) ;	// has right ngbr for sure if left swap, currently has 
			
		if (j!=l-1 and vs[i][j+1]=='-')	dashSet.insert(vector<int>{i,j+1,0});	// if right ngbr before swapping is dash, if exists
			
		if (j!=1 and vs[i][j-2]=='-')		// if left ngbr after swapping is dash, if exists
		{
			sIt it = returnDashSetItr(vector<int>{i,j-2,1});	// definitely had right ngbr
			dashSet.erase(it);									// doesn't now, so remove from set
			dashCostMatrix[i][j-2][1] = maxInt;
		}	
		
		if (j-1 == 0 or vs[i][j-2]=='-') dashCostMatrix[i][j-1][0] = maxInt;	// if does not have left non-dash ngbr
		else dashSet.insert(vector<int>{i,j-1,0}) ; 
	}
	
	else	// swap with right
	{
		leftSwap = false;
		vs[i][j] = vs[i][j+1];					// take care of edge cases. remove left/right or add left/right
		vs[i][j+1] = temp;						// just add to dashSet with inf, hillClimb recomputes values
		dashCostMatrix[i][j][0] = maxInt ; dashCostMatrix[i][j][1] = maxInt; // alphabet at place now, must have infty {INVARIANT}
		dashSet.insert(vector<int>{i,j+1,0}) ;	// has left ngbr for sure if right swap
			
		if (j!=0 and vs[i][j-1]=='-')	dashSet.insert(vector<int>{i,j-1,1});	// if left ngbr before swapping is dash, if exists {its dash cost is still infty}
			
		if (j!=l-2 and vs[i][j+2]=='-')		// if right ngbr after swapping is dash, if exists
		{
			sIt it = returnDashSetItr(vector<int>{i,j+2,0});	// definitely had left ngbr
			dashSet.erase(it);
			dashCostMatrix[i][j+2][0] = maxInt;
		}	
		
		if (j+1 == l-1 or vs[i][j+2]=='-') dashCostMatrix[i][j+1][1] = maxInt;	// if does not have right non-dash ngbr
		else dashSet.insert(vector<int>{i,j+1,1}) ; 
	}
}


//vector<string> Problem::hillClimb(vector<string>& vs, int l)			// all strings built up to length l
//{
	/* STRATEGY
	 * -------------------------------------------------
	 * Scan all dashes and see if moving left or right
	 * improves the cost at every step
	 * If it does, move to neighbour, else terminate */

	// implementation using set
	// initialise with all elements of set, if dash on left, set to infinity in dash cost matrix
	// in while loop, pop best element, see if left or right swap
	// swap, make changes to neighboring dashes (add/remove neighbours) --- also remove i,j from dashSet and insert i,j+/-1
	// if neighbouring dash is introduced, set dash move cost -> +infinity
	// recompute values for potentially four columns
	// INVARIANT :: dashSet has only those ngbrs which are possible, i.e. if a--, then right ngbr for left dash not in dashSet. also dash cost for right ngbr is infty
	// INVARIANT :: all dashCostMatrix elements other than dashes have value infty, those with dash may or may not

/*	vector<int> best(3);		// store coordinates and whether left(0) or right(1) neighbour is best
	int bestChangeYet;			// the more negative, the better 
	int loopCount = 0; 			// book-keeping
	int zeroCount = 0;			// if plateau, then for cutoff (initally 100)

	dashCostMatrix = vector<vector<vector<int> > >(noOfStrings , vector<vector<int> >(l, vector<int>(2,numeric_limits<int>::max())));

	initialiseHillClimb(vs,l);*/
	
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
	/*
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
			for (int i = 0 ; i < noOfStrings ; i++)
				if (vs[i][effX-2] == '-')
				{							
					vector<int> item = {i, effX-2 , 1};
					set<vector<int> >::iterator it = returnDashSetItr(item);
					
					if (it != dashSet.end())			// if doesn't exist, dashCostMatrix has infinite value, so goes down tree quickly
					{
						dashSet.erase(it);
					
						int finCost = columnCost(vs, i, effX-2, effX-1) + columnCost(vs, i, effX-1, effX-2);
						int curCost = columnCost(vs, i, effX-2, effX-2) + columnCost(vs, i, effX-1, effX-1);
			
						dashCostMatrix[i][effX-2][1] = finCost - curCost;
			
						item = {i, effX-2 , 1};
						dashSet.insert(item);
					}
				}
					
				
		if (effX < (l-1))			// 4th column exists
			for (int i = 0 ; i < noOfStrings ; i++)
				if (vs[i][effX+1] == '-')
				{
					vector<int> item = {i, effX+1 , 0};
					set<vector<int> >::iterator it = returnDashSetItr(item);
					
					if (it != dashSet.end())
					{
						dashSet.erase(it);
					
						int finCost = columnCost(vs, i, effX+1, effX) + columnCost(vs, i, effX, effX+1);
						int curCost = columnCost(vs, i, effX, effX) + columnCost(vs, i, effX+1, effX+1);
					
						dashCostMatrix[i][effX+1][0] = finCost - curCost;
					
						item = {i, effX+1 , 0};
						dashSet.insert(item);
					}
				}
				
		// for both effX and effX-1 , need to update lef and right move cost
		for (int i = 0 ; i < noOfStrings ; i++)
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
						
							int curRightCost = columnCost(vs, i, j, j) + columnCost(vs, i, j+1, j+1);
							int finRightCost = columnCost(vs, i ,j, j+1) + columnCost(vs, i, j+1, j);
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

							int curLeftCost = columnCost(vs, i, j, j) + columnCost(vs, i, j-1, j-1);
							int finLeftCost = columnCost(vs, i ,j, j-1) + columnCost(vs, i, j-1, j); 										
							dashCostMatrix[i][j][0] = finLeftCost - curLeftCost ;
					
							left = {i,j,0} ; 
							dashSet.insert(left) ; 
						}
					}
				}

	//////////////////////////////////    ^   updated dashes      //////////////////////
			
	// checking if column of dashes align
	//	bool check1 = true;
	//bool check2 = true;
		
		for (int q = 0 ; q<l ; q++)
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
//	}
	//cout << loopCount << endl;
	//for (int i = 0 ; i<noOfStrings ; i++)
	//	cout << vs[i] << "\n";
	
	//cout << "Final matching cost : " << allMatchingCost(vs) << endl;
	//cout << "Loop cout           : " << loopCount << endl;
/*	
	dashSet.clear();			// make ready for next iteration
	return vs;
}
*/

/*
void Problem::localSearch(int time)
{
	int minDash = 0;
	
	for (int i = 0; i<noOfStrings ; i++)
	 	minDash += maxStrLength - strLengths[i];
	
	int minDashCost = minDash*CC;
	
	int bestCostYet = firstEst();
	vector<string> bestStateYet = {"first ","is","best","pls","get","lost"};
	
	for (int loopCount = 0 ; loopCount<5000; loopCount++)
		{if (loopCount%500 == 0 and loopCount>0) cout << "loop count : " <<loopCount << endl;
		
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
				cout << "Best cost yet : " << bestCostYet << " (" << bestStateYet[0].length() << ")" <<endl;
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
				cout << "Best cost yet : " << bestCostYet << " (" << bestStateYet[0].length() << ")" <<endl;
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
				cout << "Best cost yet : " << bestCostYet << " (" << bestStateYet[0].length() << ")" <<endl;
			}
		}
	}
	
	for (int i = 0 ; i<noOfStrings ; i++)
		cout << bestStateYet[i] << endl;
	
	cout << bestCostYet << endl;
}

*/


#endif
