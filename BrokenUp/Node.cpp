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

#ifndef NODE_CPP
	#define NODE_CPP	

#include "Node.h"
#include "Problem.h"

Node::Node(int noOfStrings, int sizeOfVocab, const Problem& p)
{
	stIndices = vector<int>(noOfStrings,0);
	count     = 0;
	pathcost  = 0;
	remChar   = vector<vector<int> >(noOfStrings, vector<int>(sizeOfVocab));
	
	for (int i = 0 ; i<noOfStrings ; i++)
	{
		string curr = p.stringVector()[i];
		int l       = curr.length();

		for (int j = 0 ; j<l ; j++)
			remChar[i][p.characIndex()[(int)curr[j]]]++;
	}
	nodeCount=1;
	hst       = p.DPhst(*this);
	//printNodeDetails();
}

Node::Node(Node* par, vector<int> stateIndices, const Problem& p, int noOfStrings)
{
	stIndices    = stateIndices;
	count        = 0;
	parent       = par;
	remChar      = par->remainingChar();

	for (int i=0 ; i<noOfStrings ; i++)
		if (stIndices[i] - par->stateIndices()[i] == 1)
			remChar[i][p.characIndex()[(int)p.stringVector()[i][stateIndices[i]-1]]]--;

	pathcost  = par->path_cost() + p.pathCost(*par,*this);
	hst       = p.DPhst(*this);
	//printNodeDetails();
		nodeCount++;
	//	if (nodeCount>0 and nodeCount%5000 == 0)
	//		cout << "Node count : " << nodeCount <<endl;
}

Node::Node(const Node& rhs)
{
	pathcost  = rhs.pathcost;
	stIndices = rhs.stIndices;
	count     = 0;
	hst       = rhs.hst;
}

Node::Node(const vector<int> endStateIndices)
{
	stIndices = endStateIndices;
}

void Node::printNodeDetails()
{
	cout << "=========================== \n" ;
	cout << "State indices       : ";
	
	for (vector<int>::iterator it = stIndices.begin() ; it!= stIndices.end() ; it++)
		cout << *it << " ";
	cout << "\n";
	
	cout << "Path cost till Node : " << pathcost  << endl;
	cout << "Heuristic value     : " << hst <<endl;
	cout << "Rem char count      : { ";
	
	for (vector<vector<int> >::iterator it = remChar.begin() ; it!=remChar.end() ; it++)
		{
			cout << "( ";
			for (vector<int>::iterator itr = it->begin(); itr!= it->end() ; itr++)
				cout << *itr << " ";
			cout << ") ";
		}
		
	cout << "} \n";
	cout << "=========================== \n" ;
}

//Node::~Node()
//	{nodeCount--;}

bool Node::operator==(const Node& rhs) const
{
	int len = stIndices.size();
	
	for (int i = 0 ; i<len ; i++)
	{
		if (stIndices[i]!=rhs.stateIndices()[i])
			return false;
	}
	
	return true;		
}

bool Node::operator<(const Node& rhs) const
{
	return hst+pathcost < rhs.h() + rhs.path_cost();
}

#endif