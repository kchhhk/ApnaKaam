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

#ifndef PROBLEM_H
	#define PROBLEM_H

struct compare;
typedef std::multiset<vector<int> >::iterator sIt;

class Problem
{
	public:
		Problem(int sOfV, int K, int CC, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos);
		//~Problem();
		const vector<string>& stringVector() const {return strings;};
		const vector<int> characIndex() const {return charIndex;};
		
		const int firstEst();												// gives trivial upper bound of initial input
		//const int DPhst(const Node& node) const;
		const int getCost(char a, char b) const;							// returns the matching cost for 2 characters 
		vector<string> hillClimb(vector<string>& vs, int l);					// climbs hill for given input
		
		void printProblemDetails();											// prints all input data
		void localSearch(int time);											// in minutes, typically should run till t = time
		
	private:	
		int sizeOfVocab;
		vector<char> vocab;				// vector of characters in vocabulary
		int noOfStrings; 				// number of strings
		vector<string> strings;			// vector of strings
		vector<vector<int> > MC;		// matching cost matrix
		int CC;							// conversion cost

		vector<int> charIndex;			// position of character in Vocab and MC
		vector<int> strLengths;			// lengths of strings
		int maxStrLength;				// length of longest string

		vector<vector<vector<vector<int> > > > DPsolvedMatrix;		// DPsolve stored in DPsolvedMatrix[i][j] and [j][i]
		
		const int matchingCost(const string& s1, const string& s2); 			// computes only matching cost (excludes CC), assumes sizes are the same
		//vector<vector<int> > DPsolve(const string& s1, const string& s2);		// DP for every state 
	
		vector<string> randomStateGenerator(int size);							// generates a random state of maxL <= size <= sum of lengths

		// hill climbing helpers
		const int columnCost(const vector<string>& vs , int i , int j, int col);	// hillClimb helper, gives MC of jth alphabet of ith string with others of column col
		const int allMatchingCost(const vector<string>& vs);
		friend struct compare;
		static vector<vector<vector<int> > > dashCostMatrix;
		static multiset<vector<int>, compare > dashSet;
		void initialiseHillClimb(vector<string>& vs, int l);
		sIt returnDashSetItr(vector<int> ijk);
		void performSwap(vector<string>&vs, int l, int i, int j, int k, bool& leftSwap);
};

#endif