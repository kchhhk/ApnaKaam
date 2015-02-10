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

#ifndef NODE_H
	#define NODE_H

class Problem;

class Node
{
	public:
		Node(int noOfStrings, int sizeOfVocab, const Problem& p);							// start Node constructor
		Node(Node* par, vector<int> stateIndices, const Problem& p, int noOfStrings);		// successor constructor
		Node(const Node& rhs);																// copy constructor for storing path (ONLY!)
		Node(const vector<int> endStateIndices);											// goal Node constructor
		//~Node();
		bool operator==(const Node& rhs) const;													// only checks state indices
		bool operator<(const Node& rhs) const;
		
		Node* getParent(){return parent;}
		const int path_cost() const {return pathcost;};
		const int h() const {return hst;};
		const vector<int>& stateIndices() const {return stIndices;};
		const vector<vector<int> >& remainingChar() const {return remChar;};
		
		const int counter(){return count;};
		void incrementCounter(){count++;};
		void printNodeDetails();
		static int nodeCount ;
		static int visitCount ;

	private:
		Node* parent;
		int count;
		int pathcost;								
		int hst;									// heuristic
		vector<int> stIndices;						// k indices, ith string has been processed till index stIndices[i]
		vector<vector<int> > remChar;				// alphabet count in remaining strings
};


#endif