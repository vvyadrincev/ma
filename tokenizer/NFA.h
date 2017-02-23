#ifndef NFA_H_
#define NFA_H_
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <boost/locale/encoding_utf.hpp>
#include "RegexError.h"

using namespace std;

class NFA {
private:
	int ALPHABET_SIZE;// = 65536;
	int EPSILON;// = ALPHABET_SIZE - 1;

	vector<vector<set<int> > > transitions;
	map<int, int> finalStates;
	int startingState;

	map<int, int> insertionOrder;

	//set<int> regexpSymbols;
	//void addRegexpSymbol(const int&symbol);

	pair<int, int> parseExpression(const wstring& reg, int& index);
	pair<int, int> parseConcatenation(const wstring& reg, int& index);
	pair<int, int> parseQuantifiedElement(const wstring& reg, int& index);
	pair<int, int> parseElement(const wstring& reg, int& index);
	pair<int, int> parseSet(const wstring& reg, int& index);
	pair<int, int> parseGroup(const wstring& reg, int& index);
	pair<int, int> parseAny(const wstring& reg, int& index);
	pair<int, int> parseChar(const wstring& reg, int& index);

	vector<int> parseSetItems(const wstring& reg, int& index);
	vector<int> parseSetItem(const wstring& reg, int& index);
	vector<int> getPrebuildSet(wchar_t c);

	wchar_t getEquivalentCharacter(wchar_t c);

	void epsilonClosure(set<int>& closure, int state);
	set<int> deltabar(const set<int>& states, int symbol);
	set<int> epsilonClosureUnion(const set<int>& S, const vector<set<int> >& epsClosures);
public:
	NFA();
	NFA(int alphabet_size);
	NFA(const NFA& other);
	virtual ~NFA();
	void clear();

	const NFA& operator=(const NFA& other);

	void insertRegex(const wstring& reg, int id);
	void removeEpsilonTransitions();

	const set<int>& delta(int state, int symbol);
	int nStates() const;

    int alphabetSize() const;

	const map<int, int>& getFinalStates() const;
	const map<int, int>& getInsertionOrder() const;
	//const set<int>& getRegexpSymbols() const;
	int getStartingState() const;
	const vector<vector<set<int> > >& getTransitions() const;
	void print();
};




std::wstring utf8_to_wstring(const std::string& str);

std::string wstring_to_utf8(const std::wstring& str);

std::string unicode_to_utf8(uint32_t codepoint);

#endif
