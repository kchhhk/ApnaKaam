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




#ifndef PROBLEM_H
	#define PROBLEM_H

#include "Node.h"
#include "Problem.h"

class Problem
{
	public:
		Problem(int sOfV, int K, int CC, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos);
		//~Problem();
		Node* startNode() {return startnode;};
		Node* goalNode(){return goalnode;};
		const vector<string>& stringVector() const {return strings;};
		const vector<int> characIndex() const {return charIndex;};
		
		const int firstEst();												// gives trivial upper bound of initial input
		const int pathCost(const Node& node1, const Node& node2) const;			// returns edge cost for node1->node2 {adjacent nodes}
		const int firstHst(const Node& node) const;							// heuristic function, takes in current state indices
		const vector<Node*> successors(Node* node);							// returns nodes in sorted order of decreasing f(n)
		void printSoln(vector<Node*> pathInReverse);						// given path in reverse, start node missing
		int getCost(char a, char b) const;										// returns the matching cost for 2 characters 
		
		void printProblemDetails();											// prints all input data
		
	private:
		Node* startnode;
		Node* goalnode;
		
		int sizeOfVocab;
		vector<char> vocab;				// vector of characters in vocabulary
		int noOfStrings; 				// number of strings
		vector<string> strings;			// vector of strings
		vector<vector<int> > MC;		// matching cost matrix
		int CC;							// conversion cost

		vector<int> charIndex;			// position of character in Vocab and MC
		vector<int> strLengths;			// lengths of strings
		int maxStrLength;				// length of longest string
		vector<int> minAlphabetMC;		// min matching cost for each alphabet and - {>0}
		
		const int matchingCost(const string& s1, const string& s2); 	// computes only matching cost (excludes CC), assumes sizes are the same
};

#endif