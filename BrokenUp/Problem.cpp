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

#include "Node.h"
#include "Node.cpp"
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

	for (int i = 0 ; i<noOfStrings-1 ; i++)
		for (int j = i+1 ; j<noOfStrings ; j++)
				DPsolvedMatrix[i][j] = DPsolve(strings[i],strings[j]);

	// startnode and goalnode initialization
	startnode = new Node(noOfStrings,sOfV,*this);
	goalnode  = new Node(strLengths);
}

const int Problem::pathCost(const Node& node1, const Node& node2) const
{
	vector<int> tempStateIndex1 = node1.stateIndices();
	vector<int> tempStateIndex2 = node2.stateIndices(); 

	int pc = 0;
	string changes = "";

	for (int i=0; i<noOfStrings ;i++)
	{
		if (tempStateIndex2[i]-tempStateIndex1[i]==1)
		{
			changes +=strings[i][tempStateIndex1[i]];													//replace strings[i]
		}
		else 
		{
			changes +="-";
			pc += CC;
		}	
	}
	
	for (int i=0; i<noOfStrings-1; i++)
		for (int j=i+1; j<noOfStrings; j++)
		{
			pc+= getCost(changes[i],changes[j]);
		}
	return pc;
}

const int Problem::firstHst(const Node& node) const							// heurisitic
{//return 0;
	/* FIRST STRATEGY for heurisitic 
	 * ---------------------------------------------------------------------------------------
	 * find the longest remaining string length
	 * at the very least, all other strings must be built up to that length
	 * calculate the corresponding number of dashes to be introduced
	 * now consider strings two at a time ( k C 2), find the diff in lengths
	 * we can assume that MC=0 for diagonal elements
	 * reduce common aphabets {4G in s1, 3G in s2 --> 1G in s1}
	 * take max(summation(min each apha for s1),summation(min each alpha for s2)
	 * ---------------------------------------------------------------------------------------*/
	
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
		for (int j = i+1 ; j<noOfStrings ; j++)																	// state of indices (maxLenRem = 13)
			if (!((strLengths[i]-node.stateIndices()[i] == 0) && (strLengths[j]-node.stateIndices()[j] == 0)))
				{																								// ----------------------------------
					vector<int> remCharS1 = node.remainingChar()[i];											// { 1 , 5 , 4 } ->10
					vector<int> remCharS2 = node.remainingChar()[j];											// { 1 , 3 , 7 } ->11
					int sum1 = 0;
					int sum2 = 0;
																												// { A , B , C , - }
					remCharS1.push_back(maxLenRem - (strLengths[i]-node.stateIndices()[i]));					// { 1 , 5 , 4 , 3 }
					remCharS2.push_back(maxLenRem - (strLengths[j]-node.stateIndices()[j]));					// { 1 , 3 , 7 , 2 }
					
					for (int m = 0 ; m<sizeOfVocab+1 ; m++)
					{																							// reduced
						if (remCharS1[m] > remCharS2[m])														// { 0 , 2 , 0 , 1 }
							sum1 += (remCharS1[m] - remCharS2[m])*minAlphabetMC[m];								// { 0 , 0 , 3 , 0 }
						
						else 							
							sum2 += (remCharS2[m] - remCharS1[m])*minAlphabetMC[m];
					}
					
					h += max(sum1,sum2);
				}
			
	return h;
}

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
}

int Problem::getCost(char a, char b) const
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

const vector<Node*> Problem::successors(Node* node)
{
	vector<int> tempStateIndex = node->stateIndices();
	
	int remStrings = 0 ;
	vector<int> nonBounded;
	vector<Node*> nodeSuccessors; 
	
	for (int j=0; j<noOfStrings; j++)
	{
		if (tempStateIndex[j] < strLengths[j])
		{
			remStrings++;
			nonBounded.push_back(j);
		}
	}
	
	for (int j=1; j<pow(2,remStrings); j++)
	{
		bitset<64> stateUpdater = bitset<64>(j); 

		for (int k=0; k<remStrings; k++)
		{
			tempStateIndex[nonBounded[k]]+= stateUpdater[k];
		}
		
		Node *tempNode = new Node(node, tempStateIndex, *this, noOfStrings);
		nodeSuccessors.push_back(tempNode);
		tempStateIndex = node->stateIndices();
	}
	
	sort(nodeSuccessors.begin() , nodeSuccessors.end(), [](Node* a, Node* b){return a->path_cost() + a->h() > b->path_cost() + b->h();});	// for decreasing order	
	
	/*
	for (int i = 0 ; i<nodeSuccessors.size()-1 ; i++)
			{
				int tem = nodeSuccessors[i]->h();
				cout << "Node h(n) : " << tem << endl;
			}
	cout << "xxxxxxxxxxxxxxxxxxx" <<endl;	*/
	
	return nodeSuccessors;
}

void Problem::printSoln(vector<Node*> pathInReverse)
{
	vector<int> initialIndex(noOfStrings,0);																		
	vector<string> newstrings(noOfStrings,"");
	//reverse(pathInReverse.begin(),pathInReverse.end());
	int size=pathInReverse.size();
	Node* temp;
	for (int i=0;i<size/2;i++)
	{
		temp                    = pathInReverse[i];
		pathInReverse[i]        = pathInReverse[size-i-1];
		pathInReverse[size-i-1] = temp;
	}
	for (int i=0 ; i<pathInReverse.size(); i++)
	{
		vector<int> finalIndex = pathInReverse[i]->stateIndices();
		for (int j=0; j< noOfStrings; j++)
		{
			if (finalIndex[j] - initialIndex[j] == 1)
				newstrings[j] += strings[j][initialIndex[j]];
			else
				newstrings[j] += "-";
		}
		initialIndex = finalIndex;
	}

	for (int i = 0 ; i<noOfStrings ; i++)
		cout << newstrings[i] << endl;
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

#endif
