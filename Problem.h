class Problem
{
	public:
		Problem(int sOfV, int K, int CC, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos);
		//~Problem();
		Node* startNode() {return startnode;};
		Node* goalNode(){return goalnode;};
		const int firstEst();												// gives trivial upper bound of initial input
		const int pathCost(const Node& node1, const Node& node2);			// returns edge cost for node1->node2 {adjacent nodes}
		const int h(const vector<int>& stateIndices) const;					// heuristic function, takes in current state indices
		const vector<Node*>& successors(const Node* node);					// returns nodes in sorted order of decreasing f(n)
		void printSoln(const vector<Node*> pathInReverse);					// given path in reverse, start node missing
		void printProblemDetails();											// prints all input data
		int getCost(char a, char b);										// returns the matching cost for 2 characters 
		
	private:
		Node* startnode;
		Node* goalnode;
		
		int sizeOfVocab;
		vector<char> vocab;				// vector of characters in vocabulary
		vector<int> charIndex;			// position of character in Vocab and MC
		int noOfStrings; 				// number of strings
		vector<string> strings;			// vector of strings
		vector<int> strLengths;			// lengths of strings
		int maxStrLength;				// length of longest string
		vector<vector<int> > MC;		// matching cost matrix
		int CC;							// conversion cost
		
		const int matchingCost(const string& s1, const string& s2); 	// computes only matching cost (excludes CC), assumes sizes are the same
};