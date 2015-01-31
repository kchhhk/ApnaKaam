#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>
#include <algorithm>

using namespace std;

class Problem;

class Node
{
	public:
		Node(int noOfStrings, int sizeOfVocab, const Problem& p);		// start Node constructor
		Node(vector<int> stateIndices, int cost, const Problem& p);		// successor constructor
		Node(const Node& rhs);											// copy constructor for storing path (ONLY!)
		//~Node();
		bool operator==(const Node& rhs);								// only checks state indices
		Node* parent;

		const int path_cost(){return pathcost;};
		const int h(){return hst;};
		const vector<int>& stateIndices() const {return stIndices;};
		const vector<vector<int> >& remainingChar() const {return remChar;};
		
		const int counter(){return count;};
		void incrementCounter(){count++;};
		void printNodeDetails();

	private:
		int count;
		int pathcost;								// where are we computing pathcost??? ---> not computing, storing
		int hst;									// heuristic
		vector<int> stIndices;						// k indices, ith string has been processed till index stIndices[i]
		vector<vector<int> > remChar;				// alphabet count in remaining strings
};

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
		const int pathCost(const Node& node1, const Node& node2);			// returns edge cost for node1->node2 {adjacent nodes}
		const int h(const Node& node) const;								// heuristic function, takes in current state indices
		const vector<Node*>& successors(const Node* node);					// returns nodes in sorted order of decreasing f(n)
		void printSoln(vector<Node*> pathInReverse);						// given path in reverse, start node missing
		int getCost(char a, char b);										// returns the matching cost for 2 characters 
		
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
	
	hst       = p.h(*this);
}

Node::Node(vector<int> stateIndices, int cost, const Problem& p)
{
	pathcost  = cost;
	stIndices = stateIndices;
	count     = 0;
	hst       = p.h(*this);
}

Node::Node(const Node& rhs)
{
	pathcost  = rhs.pathcost;
	stIndices = rhs.stIndices;
	count     = 0;
	hst       = rhs.hst;
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

Problem::Problem(int sOfV, int Ks, int CCo, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos)
{
	charIndex     = vector<int>(256,-1);		//256 for each ASCII, initialised to -1 {error detection}
	minAlphabetMC = vector<int>(sOfV+1);
	
	sizeOfVocab  = sOfV;
	noOfStrings  = Ks;
	CC           = CCo;
	vocab        = V;
	strings      = str;
	MC           = MatCos;
	maxStrLength = 0;

	// char Index initialization
	for (int i = 0 ; i <sizeOfVocab ; i++)
		charIndex[(int)vocab[i]] = i;
	
	charIndex[(int)('-')] = sizeOfVocab;		// !
	
	// strLengths and maxStrLength initialization
	strLengths = vector<int>(noOfStrings);
	
	for (int i = 0 ; i<noOfStrings ; i++)
	{
		strLengths[i] = strings[i].length();
		
		if (strLengths[i]>maxStrLength)
			maxStrLength = strLengths[i];
	}
	
	// minAlphabetMC initialization
	for (int i = 0 ; i<=sizeOfVocab; i++)		// <= because dash in the end
	{
		int curmin;
		if (i!=sizeOfVocab) curmin = MC[i][i+1];
		else curmin = MC[i][i-1];
			
		for (int j = 0 ; j<=sizeOfVocab ; j++)
		{
			if (j!=i)
				if (MC[i][j] < curmin) curmin = MC[i][j];
		}
		
		minAlphabetMC[i] = curmin;
	}
	
	// startnode initialization
	startnode = new Node(noOfStrings,sOfV,*this);
	
}

const int Problem::pathCost(const Node& node1, const Node& node2)
{
	vector<int> tempStateIndex1 = node1.stateIndices();
	vector<int> tempStateIndex2 = node2.stateIndices(); 

	int pc = 0;
	string changes = "";

	for (int i=0; i<noOfStrings ;i++)
	{
		if (tempStateIndex2[i]-tempStateIndex1[i]==1)
		{
			changes +=strings[i][tempStateIndex1[i]];													//replace strings[i]
		}
		else 
		{
			changes +="-";
			pc += CC;
		}	
	}
	
	for (int i=0; i<noOfStrings-1; i++)
		for (int j=i+1; j<noOfStrings; j++)
		{
			pc+= getCost(changes[i],changes[j]);
		}
	return pc;
}

const int Problem::h(const Node& node) const							// heurisitic
{
	/* FIRST STRATEGY for heurisitic 
	 * ---------------------------------------------------------------------------------------
	 * find the longest remaining string length
	 * at the very least, all other strings must be built up to that length
	 * calculate the corresponding number of dashes to be introduced
	 * now consider strings two at a time ( k C 2), find the diff in lengths
	 * we can assume that MC=0 for diagonal elements
	 * reduce common aphabets {4G in s1, 3G in s2 --> 1G in s1}
	 * take max(summation(min each apha for s1),summation(min each alpha for s2)
	 * ---------------------------------------------------------------------------------------*/
	
	int dashes = 0;				// for CC cost, total min number of dashes to be introduced
	int maxLenRem = 0;			// longest remaining string length
	int h = 0;					// heuristic value
	
	for (int i = 0 ; i<noOfStrings ; i++)
		if (strLengths[i]-node.stateIndices()[i] > maxLenRem)
			maxLenRem = strLengths[i]-node.stateIndices()[i];
	
	for (int i = 0 ; i<noOfStrings ; i++)
		dashes += maxLenRem - (strLengths[i]-node.stateIndices()[i]);
	
	h += CC*dashes;

	for (int i = 0 ; i<noOfStrings-1 ; i++)                         						
		for (int j = i+1 ; j<noOfStrings ; j++)																		// state of indices (maxLenRem = 13)
			if (!((strLengths[i]-node.stateIndices()[i] == 0) and (strLengths[j]-node.stateIndices()[j] == 0)))
				{																									// ----------------------------------
					vector<int> remCharS1 = node.remainingChar()[i];												// { 1 , 5 , 4 } ->10
					vector<int> remCharS2 = node.remainingChar()[j];												// { 1 , 3 , 7 } ->11
					int sum1 = 0;                                                               				
					int sum2 = 0;                                                               				
																													// { A , B , C , - }
					remCharS1.push_back(maxLenRem - (strLengths[i]-node.stateIndices()[i]));						// { 1 , 5 , 4 , 3 }
					remCharS2.push_back(maxLenRem - (strLengths[j]-node.stateIndices()[j]));						// { 1 , 3 , 7 , 2 }
				                                                                                				
					for (int m = 0 ; m<sizeOfVocab+1 ; m++)                                     				
					{																								// reduced
						if (remCharS1[m] > remCharS2[m])															// { 0 , 2 , 0 , 1 }
							sum1 += (remCharS1[m] - remCharS2[m])*minAlphabetMC[m];									// { 0 , 0 , 3 , 0 }
					
						else 							
							sum2 += (remCharS2[m] - remCharS1[m])*minAlphabetMC[m];
					}
				
					h += max(sum1,sum2);
				}
			
	return h;
}

int Problem::getCost(char a, char b)
	{
		return MC[charIndex[(int)a]][charIndex[(int)b]];	
	}

const int Problem::firstEst()																		// 	try doing it heuristically
{
	vector<string> tempStrings = strings;
	int firstEstimate = 0;

	for (int i=0; i<noOfStrings; i++)														//	substitute for k by no of strings
	{
		if (strings[i].length() < maxStrLength)													//	substitute for vec by vector of strings
		{
			int noOfDashes = maxStrLength - strings[i].length();
			firstEstimate += CC * noOfDashes;

			for (int j=0; j<noOfDashes ; j++)
				tempStrings[i] += "-" ;
		} 
	}
	
	for (int i=0; i<noOfStrings-1; i++)
	{
		for (int j=i+1; j<noOfStrings; j++)
		{
			for (int k=0; k<maxStrLength; k++)
			{
				firstEstimate += getCost(tempStrings[i][k],tempStrings[j][k]);							
				//strings[i][k] and strings[j][k] matrix lookup
			}
		}
	}

	return firstEstimate;
}

const vector<Node*>& Problem::successors(const Node* node)
{}

void Problem::printSoln(vector<Node*> pathInReverse)
{
	vector<int> initialIndex(noOfStrings,0);																		
	vector<string> newstrings(noOfStrings,"");
	//reverse(pathInReverse.begin(),pathInReverse.end());
	int size=pathInReverse.size();
	Node* temp;
	for (int i=0;i<size/2;i++)
	{
		temp                    = pathInReverse[i];
		pathInReverse[i]        = pathInReverse[size-i-1];
		pathInReverse[size-i-1] = temp;
	}
	for (int i=0 ; i<pathInReverse.size(); i++)
	{
		vector<int> finalIndex = pathInReverse[i]->stateIndices();
		for (int j=0; j< noOfStrings; j++)
		{
			if (finalIndex[j] - initialIndex[j] == 1)
				newstrings[j] += strings[j][initialIndex[j]];
			else
				newstrings[j] += "-";
		}
		initialIndex = finalIndex;
	}

	for (int i = 0 ; i<noOfStrings ; i++)
		cout << newstrings[i] << endl;
}	

void Problem::printProblemDetails()
{
	cout << "=========================== \n" ;
	cout << "CC             : " << CC << "\n";
	cout << "noOfStrings    : " << noOfStrings << "\n";	
	cout << "size of vocab  : " << sizeOfVocab << "\n";
	cout << "vocab          : {";
	
	for (vector<char>::iterator it = vocab.begin() ; it!=vocab.end() ; it++ )
		cout << " " << *it << "(" << charIndex[(int)(*it)] << ")" << " " ;
	
	cout << "} \n" ; 
	cout << "strings        : ";

	for (int i = 0 ; i<noOfStrings ; i++ )
		cout << strings[i] << "[" << strLengths[i] << "]" << " " ;
	
	cout << "\n";
	cout << "MC matrix \n--------------------------- \n" ;
	
	for (vector<vector<int> >::iterator it = MC.begin() ; it!=MC.end() ; it++ )
	{
		for (vector<int>::iterator itr = it->begin() ; itr!= it->end() ; itr++ )
			cout << *itr << " ";
		
		cout << "\n";
	}
	cout << "MC matrix \n--------------------------- \n" ;
	cout << "min Alphabet wise MC  : ";
	for (vector<int>::iterator it = minAlphabetMC.begin() ; it!=minAlphabetMC.end() ; it++)
		cout << *it << " ";
	
	cout << "\n";
	
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

void DFSbb(Problem& p) 			// DFS branch & bound implementation					//!! duplicate checking??
{
	/* initialise f(n) using rudimentary calculation	
	 * work with a stack, insert in order of decreasing f(n) = g(n) + h(n) 
	 * in loop while !empty, pop and delete only after second count
	 * remember shortest path yet 
	 * if f(n) > f(n_curmin), pop & delete
	 * if StringMapProblem.goalTest(Node) then update shortest path and f(n)
	 * insert StringMapProblem.successors(Node) into stack in proper order */
	
	int bestCostYet = p.firstEst();
	vector<Node*>* bestPathYet = new vector<Node*>;				//stores in reverse order, last node first {doesn't store first node} {separate storage}
		
	stack<Node*> theStack;

	theStack.push(p.startNode());
	
	while (!theStack.empty())
	{
		Node* current = theStack.top();
		current->incrementCounter();		// nodes have initial counter 0
		
		if (current->counter() == 2)		// seen twice, delete
		{
			theStack.pop();
			delete current;
		}
		
		else if (*current == *p.goalNode())	// pop, update bestCostYet and bestPathYet
		{
			theStack.pop();
			
			if (current->path_cost() < bestCostYet)
			{
				bestCostYet = current->path_cost();

				for (vector<Node*>::iterator it = bestPathYet->begin() ; it != bestPathYet->end() ; it++)		//cleaning up previous solution
					delete *it;

				bestPathYet->resize(0);
			
				Node* temp = current;
			
				while (!(*temp == *p.startNode()))
				{
					bestPathYet->push_back(new Node(*temp));
					temp = temp->parent;
				}
			}
				
			delete current;
		}
		
		else //(current->counter() == 1)		// insert kids if f(n) doesn't exceed bestCostYet
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
	
	p.printSoln(*bestPathYet);			// prints optimal solution
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	//std::bitset<4>(10);
	
	//-------------------------------		INPUT		-------------------------------//
	
	double timeInMin;
	int sizeOfVocab, noOfStrings, CC;
	
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
	
	scanf("%i",&noOfStrings);	
	
	vector<string> strings;
	getline(cin,temp);		//to read endline
	
	for (int i = 0 ; i<noOfStrings ; i++)
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
	
	Problem* current = new Problem(sizeOfVocab,noOfStrings,CC,vocab,strings,MC);
	current->printProblemDetails();
	current->startNode()->printNodeDetails();
	cout <<current->firstEst()<<endl;
	/*vector<char> vocab = {'A','T'};
	vector<string> strings = {"ATAA","TAT","TTTTTT"};
	vector<vector<int> > MC = {{0,2,1},{2,0,1},{1,1,0}};
	
	Problem* current = new Problem(2,3,3,vocab,strings,MC);
	
//	Node(vector<int> stateIndices, int cost, const Problem& p);		// successor constructor
	
	//cout << current->firstEst() <<endl;
	
	Node* n1 = new Node(vector<int>{0,0,1},0,*current);
	Node* n2 = new Node(vector<int>{1,0,1},0,*current);
	Node* n3 = new Node(vector<int>{1,1,1},0,*current);
	Node* n4 = new Node(vector<int>{2,1,2},0,*current);
	Node* n5 = new Node(vector<int>{3,2,2},0,*current);
	Node* n6 = new Node(vector<int>{4,2,3},0,*current);
	Node* n7 = new Node(vector<int>{4,3,4},0,*current);
	Node* n8 = new Node(vector<int>{4,3,5},0,*current);
	Node* n9 = new Node(vector<int>{3,3,6},0,*current);
	
	vector<Node*> path = {n1,n2,n3,n4,n5,n6,n7,n8,n9};
	
	for (int i = 0 ; i<8 ; i++)
		cout << current->pathCost(*path[i],*path[i+1]) << " ";
	
	cout <<endl;*/
}
