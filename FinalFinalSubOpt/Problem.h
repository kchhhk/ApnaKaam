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
		
		//vector<string> hillClimb(vector<string>& vs, int l);					// climbs hill for given input
		
		void printProblemDetails();											// prints all input data
		//void localSearch(int time);										// in minutes, typically should run till t = time
		void magicLocalSearch(int time);
		
		void magic(vector<string>& vs, const vector<int>& orderArray, int start, int end, int loopCount);
		
		
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

		// vector<vector<vector<vector<int> > > > DPsolvedMatrix;		// DPsolve stored in DPsolvedMatrix[i][j], i<j
		vector<vector<pair<string,string> > > DPfinalString;			// stores best soln from start state from DP
		
		const int matchingCost(const string& s1, const string& s2); 			// computes only matching cost (excludes CC), assumes sizes are the same
		pair<string,string> DPsolve(const string& s1, const string& s2);		// final DP solution stored in DPfinalString
	
		vector<string> randomStateGenerator(int size);							// generates a random state of maxL <= size <= sum of lengths

		// hill climbing helpers
		const int columnCost(const vector<string>& vs , int i , int j, int col);	// hillClimb helper, gives MC of jth alphabet of ith string with others of column col
		const int columnCost(const vector<string>& vs , int i , int j, int col , int start, int end);
		const int allMatchingCost(const vector<string>& vs);
		const int allMatchingCost(const vector<string>& vs, int start, int finish);
		friend struct compare;
		static vector<vector<vector<int> > > dashCostMatrix;
		static multiset<vector<int>, compare > dashSet;
		void initialiseHillClimb(vector<string>& vs, int l, int start, int end);
		sIt returnDashSetItr(vector<int> ijk);
		void performSwap(vector<string>&vs, int l, int i, int j, int k, bool& leftSwap);
		
		// magic helpers
		void fitNewString(vector<string>& vs, const vector<int>& orderArray , int start, int end, int maxLength, int loopCount);
		vector<string> informedRandomState(vector<string>& vs, const vector<int>& orderArray, int start, int end, int maxLength);
		void blockAlign(vector<string>& cur, const vector<int>& orderArray, int start, int end, int length);
		int magicHillClimb(vector<string>& vs, int start, int end);								
};

#endif