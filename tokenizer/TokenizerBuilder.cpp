#include "TokenizerBuilder.h"
#include "Tokenizer.h"


TokenizerBuilder::TokenizerBuilder() {
    ALPHABET_SIZE = 4096;
    nfa = NFA(ALPHABET_SIZE);
}

TokenizerBuilder::TokenizerBuilder(int alphabet_size) {
    ALPHABET_SIZE = alphabet_size;
    this->nfa = NFA(ALPHABET_SIZE);
}

TokenizerBuilder::~TokenizerBuilder() {

}

void TokenizerBuilder::addTokenType(int tokenId, const wstring& regularExpression) {
	if(tokenId < 0) {
		throw invalid_argument("tokenId cannot be negative");
	}
	nfa.insertRegex(regularExpression, tokenId);
}

Tokenizer TokenizerBuilder::build(bool add_often_using_symbols) {
	nfa.removeEpsilonTransitions();
	DFA dfa(nfa, add_often_using_symbols);
    this->nfa.clear();
    map<int, map<int, int>> stateSymbPos = dfa.getStateSymbPos();
	//dfa.minimizeStates();
	return Tokenizer(dfa, stateSymbPos);
}


