#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "Token.h"
#include "TokenizerBuilder.h"
#include "DFA.h"
#include <istream>
#include <iostream>
#include "StringStream.h"

class Tokenizer {
	DFA dfa;

	StringStream stream;

	bool prepared;
	bool finished;
	Token token;
	int tokenlen;
	int lineNumbersCounter;
	int characterPositionCounter;

	int characterAbsPositionCounter;

	set<pair<int,int> > hopeless;

	void prepareToken();
	void checkPrepared();

	Tokenizer(const DFA& dfa);
	//Tokenizer(const DFA& dfa, set<int>& regexSymbols);
	Tokenizer(const DFA& dfa, map<int, map<int, int>> stateSymbPos);
	void init(const DFA& dfa);

	friend class TokenizerBuilder;

public:
	virtual ~Tokenizer();

	void setInputStream(const wstring& ws);
	void reset();

	void printDFA();

	bool hasNext();
	Token nextToken();
};

#endif
