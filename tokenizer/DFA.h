#ifndef DETERMINISTICFINITEAUTOMATA_H_
#define DETERMINISTICFINITEAUTOMATA_H_
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include "DFASet.h"
#include "NFA.h"

using namespace std;

class DFA {
private:
	const int NULL_STATE = 1;

	//vector<vector<int> > transitions;
	map<int,map<int, int>> stateSymbPos;
	map<int, int> finalStates;
	int startingState;
    set<int> regexSymbols;
	int ALPHABET_SIZE;

	void init(const NFA& nfa);
	void add_often_using_symbols(const vector<std::wstring>);

public:
	DFA();
	DFA(const DFA& other);
	DFA(const NFA& nfa);
	DFA(const NFA& nfa, const bool add_often_using_symbols);
	virtual ~DFA();

	const DFA& operator=(const DFA& other);
	void minimizeStates();

	int delta(int state, int symbol) const;
	int getStartingState() const;
	int nStates() const;
	bool isFinalState(int state) const;
	int getFinalStateId(int state) const;

    map<int,map<int, int>> getStateSymbPos();
    void setStateSymbPos(map<int,map<int, int>>& stateSymbPos);

    //set<int> getRegexSymbols();
    //void setRegexSymbols(set<int>& regexSymbols);

	bool isNullState(int state) const;
	void print();
};

#endif

