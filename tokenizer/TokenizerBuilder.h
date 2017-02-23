#ifndef TOKENIZERBUILDER_H_
#define TOKENIZERBUILDER_H_

#include <string>
#include "NFA.h"
using namespace std;

class Tokenizer;
class TokenizerBuilder {
private:
	NFA nfa;
	int ALPHABET_SIZE;
public:
	TokenizerBuilder();
	TokenizerBuilder(int alphabet_size);

	virtual ~TokenizerBuilder();

	void addTokenType(int tokenIdentifier, const wstring& regularExpression);
	Tokenizer build(bool add_often_using_symbols);
};
#endif
