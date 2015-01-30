

Node::Node(int k, const Problem& p)
{
	stIndices = vector<int>(k,0);
	count     = 0;
	pathcost  = 0;
	hst       = p.h(stIndices);		// make correction	
}

Node::Node(vector<int> stateIndices, int cost, const Problem& p)
{
	pathcost  = cost;
	stIndices = stateIndices;
	count     = 0;
	hst       = p.h(stIndices);		// make correction	
}

Node::Node(const Node& rhs)
{
	pathcost  = rhs.pathcost;
	stIndices = rhs.stIndices;
	count     = 0;
	hst       = rhs.hst;
}

bool Node::operator==(const Node& rhs)
{
	int len = stIndices.size();
	
	for (int i = 0 ; i<len ; i++)
	{
		if (stIndices[i]!=rhs.stateIndices()[i])
			return false;
	}
	
	return true;		
}


