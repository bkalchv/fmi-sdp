#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct Domino
{
	int left;
	int right;

	Domino(int _left, int _right) : left{ _left }, right{ _right } {};

	void print() const
	{
		cout << "[" << left << "||" << right << "]";
	}
	bool operator==(const Domino& other) const
	{
		return (left == other.left && right == other.right);
	}
	bool operator!=(const Domino& other) const
	{
		return !operator==(other);
	}
	bool operator<(const Domino& other)
	{
		if (left == other.left)
		{
			return right < other.right;
		}
		else
		{
			return left < other.left;
		}
	}
};

bool isDigit(char c)
{
	return (c > 47 || c < 58);
}

void printDominoSequence(const list<Domino>& ds) 
{
	for (const Domino& d : ds)
	{
		d.print();
		if (d != ds.back())
		{
			cout << " - ";
		}
	}
	cout << endl;
}

bool isValidDominoSequence(const list<Domino>& ds)
{
	if (ds.size() < 2) return false;

	list<Domino>::const_iterator it = ds.begin();
	for (list<Domino>::const_iterator itnext{++ds.begin()}; itnext != ds.end(); ++itnext)
	{
		if (it->right != itnext->left)
		{
			return false;
		}
		++it;
	}

	return true;
}

vector<list<Domino>> getAllPossiblePermutations(list<Domino> ds)
{
	ds.sort();
	vector<list<Domino>> result;
	do
	{
		result.push_back(ds);
	} while (std::next_permutation(ds.begin(), ds.end()));

	return result;
}

list<Domino> getLongestDominoSequenceOfPermutation(const list<Domino>& ds)
{
	for (size_t i{0}; i < ds.size(); ++i)
	{
		auto first = next(ds.begin(), i);
		auto last  = ds.end();
		std::list<Domino> subDs(first, last);
		if (isValidDominoSequence(subDs))
		{
			return subDs;
		}
	}
	return list<Domino>();
}

list<Domino> getLongestDominoSequence(const vector<list<Domino>>& allPossibilites)
{
	size_t currentLongestIndex = 0;
	for (size_t i{1}; i < allPossibilites.size();++i)
	{
		if (allPossibilites[i].size() > allPossibilites[currentLongestIndex].size())
		{
			currentLongestIndex = i;
		}
	}

	return allPossibilites[currentLongestIndex];
}

vector<list<Domino>> getAllPossibleDominoSequences(list<Domino> ds)
{
	vector<list<Domino>> permutations = getAllPossiblePermutations(ds);

	vector<list<Domino>> result;

	for (const list<Domino>& l : permutations)
	{
		list<Domino> longestDominoSequence = getLongestDominoSequenceOfPermutation(l);
		if (longestDominoSequence.size() != 0)
		{
			result.push_back(longestDominoSequence);
		}
	}

	return result;
}

int main()
{
	// Dominoes task
	list<Domino> dominoSequence;

	string dominosFilename = "Dominos.txt";
	ifstream dominosInputFile(dominosFilename);
	if (dominosInputFile)
	{
		string line;
		while (getline(dominosInputFile, line))
		{
			istringstream iss(line);
			string domino;
			if (iss >> domino && domino.size() == 2)
			{
				char left		= domino[0];
				char right		= domino[1];
				
				if (isDigit(left) && isDigit(right))
				{
					Domino currentDomino(left - '0', right - '0');
					dominoSequence.push_back(currentDomino);
				}
				else
				{
					cout << "Faulty input from " << dominosFilename << endl;
				}
			}
		}
		cout << "Domino sequence successfully read!" << endl;
	}
	else
	{
		cout << "Dominos file couldn't be open!" << endl;
	}

	printDominoSequence(dominoSequence);

	bool isValid = isValidDominoSequence(dominoSequence);
	isValid ? cout << "Valid " : cout << "Invalid "; cout << " domino sequence."; cout << endl;
	
	if (!isValid)
	{
		vector<list<Domino>> allPossibleDominoSequences = getAllPossibleDominoSequences(dominoSequence);
		list<Domino> longestDominoSequence = getLongestDominoSequence(allPossibleDominoSequences);
		printDominoSequence(longestDominoSequence);
	}

	return 0;
}