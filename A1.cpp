#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
#include <stack>

using namespace std;

///////////////////////////////////////////////////		STRING MAPPING		///////////////////////////////////////////////////



class State
{
	public:
		State(){};
		//~State();
		
	private:
		
};

class Node
{
	public:
		Node(){};
		//~Node();
		Node* parent;
		
	private:
		
};

class Problem
{
	public:
		Problem(int sOfV, int K, int CC, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos);
		//~Problem();
		const int path_cost(const State& state1, const State& state2);		//returns edge cost for state1->state2 {adjacent states}
		const int h(const State& state);									// heuristic function
		const vector<Node*> successors(const Node& node);
		
		void printProblemDetails();											// prints all input data
		
	private:
		State startState;
		State goalState;
		
		int sizeOfVocab;
		vector<char> vocab;				// vector of characters in vocabulary
		vector<int> charIndex;			// position of character in Vocab and MC
		int K; 							// number of strings
		vector<string> strings;			// vector of strings
		vector<int> strLengths;			// lengths of strings
		vector<vector<int> > MC;		// matching cost matrix
		int CC;							// conversion cost
		
		const int matchingCost(const string& s1, const string& s2); 	// computes only matching cost (excludes CC), assumes sizes are the same

};

Problem::Problem(int sOfV, int Ks, int CCo, const vector<char>& V, const vector<string>& str, const vector<vector<int> >& MatCos)
{
	charIndex = vector<int>(256,-1);		//256 for each ASCII, initialised to -1 {error detection}
	
	sizeOfVocab = sOfV;
	K           = Ks;
	CC          = CCo;
	vocab       = V;
	strings     = str;
	MC          = MatCos;

	for (int i = 0 ; i <sizeOfVocab ; i++)
		charIndex[(int)vocab[i]] = i;
	
	charIndex[(int)('-')] = sizeOfVocab;		// !
	
	strLengths  = vector<int>(K);
	
	for (int i = 0 ; i<K ; i++)
		strLengths[i] = strings[i].length();
	
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

void DFSbb(const Problem& smp) 			// DFS branch & bound implementation
{
	// initialise f(n) using rudimentary calculation
	
	// work with a stack, insert in order of decreasing f(n) = g(n) + h(n) {only those with}
	
	// in loop while !empty, pop and delete only after second count
	
	// remember shortest path yet 
	
	// if f(n) > f(n_curmin), pop & delete
	
	// if StringMapProblem.goalTest(Node) then update shortest path and f(n)
	
	// insert StringMapProblem.successors(Node) into stack in proper order
};


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
