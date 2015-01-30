#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>

using namespace std;

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
	
	p.printSoln(bestPathYet);			// prints optimal solution
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	//std::bitset<4>(10);
	
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
