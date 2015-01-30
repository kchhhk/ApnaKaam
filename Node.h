class Node
{
	public:
		Node(int k, const Problem& p);									// start Node constructor
		Node(vector<int> stateIndices, int cost, const Problem& p);		// successor constructor
		Node(const Node& rhs);											// copy constructor for storing path
		//~Node();
		bool operator==(const Node& rhs);								// only checks state indices
		Node* parent;

		int path_cost(){return pathcost;};
		int h(){return hst;};
		const vector<int>& stateIndices() const {return stIndices;};
		
		const int counter(){return count;};
		void incrementCounter(){count++;};


	private:
		int count;
		int pathcost;													//where are we computing pathcost???
		int hst;														//heuristic
		vector<int> stIndices;											// k indices, ith string has been processed till index stIndices[i]
};