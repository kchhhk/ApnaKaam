#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>

using namespace std;

///////////////////////////////////////////////////		STRING MAPPING		///////////////////////////////////////////////////

class Node
{
	public:
		Node(int k);									// start Node constructor
		Node(vector<int> stateIndices, int cost);		// successor constructor
		Node(const Node& rhs);							// copy constructor for storing path
		//~Node();
		bool operator==(const Node& rhs);				// only checks state indices
		Node* parent;
		int path_cost(){return pathcost;};
		int h(){return hst;};
		const vector<int>& stateIndices(){return stIndices;};

	private:
		int counter;
		int pathcost;
		int hst;				//heuristic
		vector<int> stIndices;							// k indices, ith string has been processed till index stIndices[i]
};

class Problem
{
	public:
		Problem(int sOfV, int K, int CC, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos);
		//~Problem();
		Node* startNode(){return startnode;};
		Node* goalNode(){return goalnode;};
		const int firstEst();												// gives trivial upper bound of initial input
		const int path_cost(const Node& node1, const Node& node2);			// returns edge cost for node1->node2 {adjacent nodes}
		const int h(const vector<int>& stateIndices);						// heuristic function, takes in current state indices
		const vector<Node*> successors(const Node& node);					// returns nodes in sorted order of decreasing f(n)
		void printSoln(const vector<Node*>& pathInReverse);					// given path in reverse, start node missing
		void printProblemDetails();											// prints all input data
		
	private:
		Node* startnode;
		Node* goalnode;
		
		int sizeOfVocab;
		vector<char> vocab;				// vector of characters in vocabulary
		vector<int> charIndex;			// position of character in Vocab and MC
		int K; 							// number of strings
		vector<string> strings;			// vector of strings
		vector<int> strLengths;			// lengths of strings
		int maxStrLength;				// length of longest string
		vector<vector<int> > MC;		// matching cost matrix
		int CC;							// conversion cost
		
		const int matchingCost(const string& s1, const string& s2); 	// computes only matching cost (excludes CC), assumes sizes are the same
};

Node::Node(int k)
{
	stIndices = vector<int>(k,0);
	counter   = 0;
	pathcost  = 0;
	hst       = Problem.h(stIndices);		// make correction	
}

Node::Node(vector<int> stateIndices, int cost)
{
	pathcost  = cost;
	stIndices = stateIndices;
	counter    = 0;
	hst       = Problem.h(stIndices);		// make correction	
}

Node::Node(const Node& rhs)
{
	pathcost  = rhs.pathcost;
	stIndices = rhs.stIndices;
	counter   = 0;
	hst       = rhs.hst;
}

bool Node::operator==(const Node& rhs)
{
	
}

Problem::Problem(int sOfV, int Ks, int CCo, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos)
{
	charIndex = vector<int>(256,-1);		//256 for each ASCII, initialised to -1 {error detection}
	
	sizeOfVocab  = sOfV;
	K            = Ks;
	CC           = CCo;
	vocab        = V;
	strings      = str;
	MC           = MatCos;
	maxStrLength = 0;

	for (int i = 0 ; i <sizeOfVocab ; i++)
		charIndex[(int)vocab[i]] = i;
	
	charIndex[(int)('-')] = sizeOfVocab;		// !
	
	strLengths  = vector<int>(K);
	
	for (int i = 0 ; i<K ; i++)
	{
		strLengths[i] = strings[i].length();
		
		if (strLengths[i]>maxStrLength)
			maxStrLength = strLengths[i];
	}
	
}

void Problem::printProblemDetails()
{
	cout << "=========================== \n" ;
	cout << "CC             : " << CC << "\n";
	cout << "K              : " << K << "\n";	
	cout << "size of vocab  : " << sizeOfVocab << "\n";
	cout << "vocab          : {";
	
	for (vector<char>::iterator it = vocab.begin() ; it!=vocab.end() ; it++ )
		cout << " " << *it << "(" << charIndex[(int)(*it)] << ")" << " " ;
	
	cout << "} \n" ; 
	cout << "strings        : ";

	for (int i = 0 ; i<K ; i++ )
		cout << strings[i] << "[" << strLengths[i] << "]" << " " ;
	
	cout << "\n";
	cout << "MC matrix \n--------------------------- \n" ;
	
	for (vector<vector<int> >::iterator it = MC.begin() ; it!=MC.end() ; it++ )
	{
		for (vector<int>::iterator itr = it->begin() ; itr!= it->end() ; itr++ )
			cout << *itr << " ";
		
		cout << "\n";
	}
	
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DFSbb(const Problem& p) 			// DFS branch & bound implementation					//!! duplicate checking??
{
	/* initialise f(n) using rudimentary calculation	
	 * work with a stack, insert in order of decreasing f(n) = g(n) + h(n) 
	 * in loop while !empty, pop and delete only after second count
	 * remember shortest path yet 
	 * if f(n) > f(n_curmin), pop & delete
	 * if StringMapProblem.goalTest(Node) then update shortest path and f(n)
	 * insert StringMapProblem.successors(Node) into stack in proper order */
	
	int bestCostYet = p.firstEst();
	vector<Node*>* bestPathYet = new vector<Node*>;				//stores in reverse order, last node first {doesn't store first node}
		
	stack<Node*> theStack;

	theStack.push(p.startNode())
	
	while (!theStack.empty())
	{
		Node* current = theStack.top();
		current->counter++;					// nodes have initial counter 0
		
		if (current->counter == 2)			// seen twice, delete
		{
			theStack.pop();
			delete current;
		}
		
		else if (*current == p.goalNode())	// pop, update bestCostYet and bestPathYet
		{
			theStack.pop();
			
			if (current->path_cost() < bestCostYet)
			{
				bestCostYet = current->path_cost();

				for (vector<Node*>::iterator it = bestPathYet->begin() ; it != bestPathYet->end() ; it++)		//cleaning up previous solution
					delete *it;

				bestPathYet->resize(0);
			
				Node* temp = current;
			
				while ((*temp) != p.startNode())
				{
					bestPathYet.push_back(new Node(*temp));
					temp = temp->parent;
				}
			}
				
			delete current;
		}
		
		else //(current->counter == 1)		// insert kids if f(n) doesn't exceed bestCostYet
		{
			if (current->path_cost() + current->h() >= bestCostYet)		// what if initial bestCostYet is best bestCostYet?
			{
				theStack.pop();
				delete current;
			}
			
			else							// push successors into stack, set parents to current
			{
				vector<Node*> temp = p.successors(current);				// ASSERT: decreasing order of f(n)
				
				for (vector<Node*>::iterator it = temp.begin() ; it != temp.end() ; it++)
				{
					theStack.push(*it);
					(*it)->parent = current;
				}
			}
		}		
	}
	
	printSoln(bestPathYet);			// prints optimal solution
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	//std::bitset<4>(100);
	
	//-------------------------------		INPUT		-------------------------------//
	
	double timeInMin;
	int sizeOfVocab, K, CC;
	
	scanf("%lf",&timeInMin);
	scanf("%i",&sizeOfVocab);	
	
	vector<char> vocab;
	string temp;
	getline(cin,temp);		//to read endline
	getline(cin,temp);
	
	for (string::iterator it = temp.begin() ; it!=temp.end() ; it++)
	{
		if ((char)(*it)!=',' and (char)(*it)!=' ')
			vocab.push_back((char)(*it));
	}
	
	scanf("%i",&K);	
	
	vector<string> strings;
	getline(cin,temp);		//to read endline
	
	for (int i = 0 ; i<K ; i++)
	{
		getline(cin,temp);
		strings.push_back(temp);
	}

	scanf("%i",&CC);	
	
	vector<vector<int> > MC(sizeOfVocab+1 , vector<int>(sizeOfVocab+1));
	
	for (int i = 0 ; i<sizeOfVocab+1 ; i++)
	{		
		for (int j = 0 ; j<sizeOfVocab+1 ; j++)
		{
			int tem;
			scanf("%i", &tem);
			MC[i][j] = tem;
		}
	}
	
	char end;
	scanf("%c",&end);		//reads endline char
	scanf("%c",&end); 
	
//-------------------------------------------------------------------------------//

//	if (end == '#')
	
	Problem* current = new Problem(sizeOfVocab,K,CC,vocab,strings,MC);
	current->printProblemDetails();
	
}
