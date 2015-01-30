

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
	
	strLengths = vector<int>(K);
	
	for (int i = 0 ; i<K ; i++)
	{
		strLengths[i] = strings[i].length();
		
		if (strLengths[i]>maxStrLength)
			maxStrLength = strLengths[i];
	}
	
}

const int Problem::pathCost(const Node& node1, const Node& node2)
{
	vector<Node*> tempStateIndex1 = node1.stateIndices();
	vector<Node*> tempStateIndex2 = node2.stateIndices(); 

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
				firstEstimate += getCost(tempStrings[i][k],tempStrings[j][k]);													//strings[i][k] and strings[j][k] matrix lookup
			}
		}
	}

	return firstEstimate;
}

void Problem::printSoln(const vector<Node*> pathInReverse)
{
	vector<int> initialIndex(noOfStrings,0);																		
	vector<string> newstrings(noOfStrings,"");
	pathInReverse.reverse();

	for (int i=0; i<pathInReverse.size(); i++)
	{
		vector<int> finalIndex = pathInReverse[i].stateIndices();
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
		{
			printf(%s,newstrings[i]);
			cout<<endl; 
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