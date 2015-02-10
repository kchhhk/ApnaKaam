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

#ifndef PROBENGINE_H
	#define PROBENGINE_H

// it's the part that can make or break out Suboptimal Search
// it's the part that can learn
// it's our, very own. Probability Engine.
// macha de
class probEngine
{
	public:
		probEngine(const vector<int>& strLengths);					// decide what else you need for input				
		const int nextSize();										// the prob distribution of this changes with time
		void update(int length, int cost, int noOfLoops, ...?);		// we throw new info after every iteration
		void penalize(int initialLength, int finalLength, ...?);	// if column of dashes from initial length, reduce it's probability
	
	private:
		vector<int> strLengths;		// stores length of strings
};

// you could take initial info such as CC, cost matrix to create an initial prob distribution and modify it accordingly
// length also depends on how different lengths of strings are, i.e. if all are similar or very different. soch le.
// try to get a mathematically sound model
// only output is nextSize()
// add whatever you see fit

