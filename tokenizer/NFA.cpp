#include "NFA.h"
#include <iostream>


NFA::NFA() {
    ALPHABET_SIZE = 8192;
    EPSILON = ALPHABET_SIZE - 1;
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	startingState = 0;
}

NFA::NFA(int alphabet_size) {
    ALPHABET_SIZE = alphabet_size;
    EPSILON = ALPHABET_SIZE - 1;
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	startingState = 0;
}

NFA::~NFA() {

}

void NFA::clear(){
    this->transitions.clear();
}


NFA::NFA(const NFA& other) {
	this->transitions = other.transitions;
	this->finalStates = other.finalStates;
	this->startingState = other.startingState;
}

const NFA& NFA::operator =(const NFA& other) {
	if(this != &other) {
		this->transitions = other.transitions;
		this->finalStates = other.finalStates;
		this->startingState = other.startingState;
	}
	return *this;
}

int NFA::alphabetSize() const{
    return this->ALPHABET_SIZE;
}

void NFA::insertRegex(const wstring& reg, int id) {
	if(insertionOrder.count(id)) {
		stringstream ss;
		ss << "Token id: " << id << " already inserted";
		throw invalid_argument(ss.str());
	}

	insertionOrder[id] = insertionOrder.size() + 1;

	int index = 0;
	pair<int, int> a = parseExpression(reg, index);
	if(index != reg.size()) {
		throw RegexError("couldn't complete parsing");
	}

	transitions[startingState][EPSILON].insert(a.first);
	finalStates.insert(make_pair(a.second, id));
}



void NFA::epsilonClosure(set<int>& closure, int state) {
	if(!closure.count(state)) {
		closure.insert(state);
		const set<int>& s = delta(state, EPSILON);
		for(set<int>::iterator i = s.begin(); i != s.end(); i++) {
			epsilonClosure(closure, *i);
		}
	}
}

set<int> NFA::epsilonClosureUnion(const set<int>& S, const vector<set<int> >& epsClosures) {
	set<int> result;
	for(set<int>::iterator i = S.begin(); i != S.end(); i++) {
		for(set<int>::iterator j = epsClosures[*i].begin(); j != epsClosures[*i].end(); j++) {
			result.insert(*j);
		}
	}
	return result;
}

void NFA::print() {
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "states: " << nStates() << endl;
	cout << "finals: " << finalStates.size() << endl;
	cout << "start:  " << startingState << endl;

    //set::iterator sym;
//    for(std::set<int>::iterator sym=regexpSymbols.begin(); sym!=regexpSymbols.end();++sym) {
      //cout << *sym << '\n';
//    }

    cout << '\n';


	for (int i = 0; i < nStates(); i++) {
		cout << "    " << i;
		if (finalStates.count(i))
			cout << "\tF\n";
		else
			cout << "\tN\n";
	}
}

void NFA::removeEpsilonTransitions() {
	vector<set<int> > epsClosures(nStates());

	for(int i = 0; i < epsClosures.size(); i++) {
		epsilonClosure(epsClosures[i], i);
	}

	vector<vector<set<int> > > deltaPrime(nStates(), vector<set<int> >(ALPHABET_SIZE));
	for(int i = 0; i < nStates(); i++) {
		for(int j = 0; j < ALPHABET_SIZE; j++) {
			if(j != EPSILON) {
				deltaPrime[i][j] = epsilonClosureUnion(deltabar(epsClosures[i], j), epsClosures);
			}
		}
	}
	transitions = deltaPrime;

	map<int, int> finalStatesPrime;
	const int oo = 0x7fffffff;
	for(int i = 0; i < epsClosures.size(); i++) {
		int bestId = -1;
		int bestIdOrder = oo;
		for(set<int>::iterator j = epsClosures[i].begin(); j != epsClosures[i].end(); j++) {
			int state = *j;
			if(finalStates.count(state)) {
				int id = finalStates[state];
				int idOrder = insertionOrder[id];
				if(idOrder < bestIdOrder) {
					bestIdOrder = idOrder;
					bestId = id;
				}
			}
		}
		if(bestIdOrder != oo) {
			finalStatesPrime[i] = bestId;
		}
	}
	finalStates = finalStatesPrime;
	//print();
}

const set<int>& NFA::delta(int state, int symbol) {
	return transitions[state][symbol];
}

set<int> NFA::deltabar(const set<int>& states, int symbol) {
	set<int> result;
	for(set<int>::iterator i = states.begin(); i != states.end(); i++) {
		const set<int>& x = delta(*i, symbol);
		for(set<int>::iterator j = x.begin(); j != x.end(); j++) {
			result.insert(*j);
		}
	}
	return result;
}

int NFA::nStates() const {
	return transitions.size();
}

const map<int, int>& NFA::getFinalStates() const {
	return finalStates;
}

const map<int, int>& NFA::getInsertionOrder() const {
	return insertionOrder;
}

int NFA::getStartingState() const {
	return startingState;
}

const vector<vector<set<int> > >& NFA::getTransitions() const {
	return transitions;
}


//	<exp>	-> <conc> { "|" <conc> }
pair<int, int> NFA::parseExpression(const wstring& reg, int& index) {

	vector<pair<int, int> > concatenations;
	pair<int, int> a = parseConcatenation(reg, index);
	concatenations.push_back(a);
	while(index < reg.size() && reg[index] == L'|') {
		index++;
		a = parseConcatenation(reg, index);
		concatenations.push_back(a);
	}

	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	for(int i = 0; i < concatenations.size(); i++) {
		transitions[startIndex][EPSILON].insert(concatenations[i].first);
		transitions[concatenations[i].second][EPSILON].insert(endIndex);
	}
	return make_pair(startIndex, endIndex);
}

//	<conc>	-> <qelm> { <qelm> }
pair<int, int> NFA::parseConcatenation(const wstring& reg, int& index) {
	vector<pair<int, int> > elements;
	pair<int, int> a = parseQuantifiedElement(reg, index);
	elements.push_back(a);
	while(index < reg.size() && reg[index] != L'|') {
		a = parseQuantifiedElement(reg, index);
		elements.push_back(a);
	}
	for(int i = 0; i < (int)elements.size() - 1; i++) {
		transitions[elements[i].second][EPSILON].insert(elements[i + 1].first);
	}
	return make_pair(elements[0].first, elements[elements.size() - 1].second);
}

//	<qelm>	-> <elem> | <elem> "*" | <elem> "+" | <elem> "?"
pair<int, int> NFA::parseQuantifiedElement(const wstring& reg, int& index) {
	pair<int, int> a = parseElement(reg, index);
	if(index < reg.size()) {
		if(reg[index] == L'?') {
			transitions[a.first][EPSILON].insert(a.second);
			index++;
		} else if(reg[index] == L'*') {
			int startIndex = nStates();
			transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
			transitions[startIndex][EPSILON].insert(a.first);
			transitions[startIndex][EPSILON].insert(a.second);
			transitions[a.second][EPSILON].insert(a.first);
			a = make_pair(startIndex, a.second);
			index++;
		} else if(reg[index] == L'+') {
			transitions[a.second][EPSILON].insert(a.first);
			index++;
		}
	}
	return a;
}

//	<elem>	-> <group> | <any> | <char> | <set>
pair<int, int> NFA::parseElement(const wstring& reg, int& index) {
	if(index < reg.size()) {

		if(reg[index] == L'(') {
			return parseGroup(reg, index);
		} else if(reg[index] == L'.') {
			return parseAny(reg, index);
		} else if(reg[index] == L'[') {
			return parseSet(reg, index);
		} else {
			return parseChar(reg, index);
		}
	} else {
		return make_pair(-1, -1);
	}
}

//	<set>	-> "[" <setitems> "]" | "[^" <setitems> "]"
pair<int, int> NFA::parseSet(const wstring& reg, int& index) {
	if(reg[index] != L'[') throw RegexError(string("Expected: '[' found: \'") + unicode_to_utf8(reg[index]) + "\'");
	index++;
	bool negative = false;
	if(index < reg.size() && reg[index] == L'^') {
		index++;
		negative = true;
	}
	vector<int> a = parseSetItems(reg, index);
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));

	if(negative) {
		for(int i = 0; i < ALPHABET_SIZE; i++) {
			transitions[startIndex][i].insert(endIndex);
		}
		transitions[startIndex][EPSILON].erase(endIndex);
		for(int i = 0; i < a.size(); i++) {
			transitions[startIndex][a[i]].erase(endIndex);
		}
	} else {
		for(int i = 0; i < a.size(); i++) {
			transitions[startIndex][a[i]].insert(endIndex);
		}
	}

	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	if(reg[index] != L']') throw RegexError(string("Expected: ']' found: \'") + unicode_to_utf8(reg[index]) + "\'");
	else index++;
	return make_pair(startIndex, endIndex);
}

//	<group>	-> "(" <exp> ")"
pair<int, int> NFA::parseGroup(const wstring& reg, int& index) {
	if(reg[index] != L'(') throw RegexError(string("Expected: '(' found: \'") + unicode_to_utf8(reg[index]) + "\'");
	index++;
	pair<int, int> a = parseExpression(reg, index);
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	if(reg[index] != L')') throw RegexError(string("Expected: ')' found: \'") + unicode_to_utf8(reg[index]) + "\'");
	else index++;
	return a;
}

//	<any>	-> "."
pair<int, int> NFA::parseAny(const wstring& reg, int& index) {
	if(reg[index] != L'.') throw RegexError(string("Expected: '.' found: \'") + unicode_to_utf8(reg[index]) + "\'");
	index++;
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));

	for(int i = 0; i < ALPHABET_SIZE; i++) {
		transitions[startIndex][i].insert(endIndex);
	}
	transitions[startIndex][EPSILON].erase(endIndex);
	return make_pair(startIndex, endIndex);
}

//	<setitems>	-> <setitem> { <setitem> }
vector<int> NFA::parseSetItems(const wstring& reg, int& index) {
	vector<int> values;
	vector<int> a = parseSetItem(reg, index);
	values.insert(values.end(), a.begin(), a.end());
	while(index < reg.size() && reg[index] != L']') {
		a = parseSetItem(reg, index);
		values.insert(values.end(), a.begin(), a.end());
	}
	return values;
}

//	<setitem>	-> <char> | "\" <char>
vector<int> NFA::parseSetItem(const wstring& reg, int& index) {
	vector<int> A;
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	wchar_t x = reg[index++];
	if(x == L'\\') {
		if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
		x = reg[index++];
		if(tolower(x) == L'd' || tolower(x) == L'w' || tolower(x) == L's') {
			return getPrebuildSet(x);
		}
		x = getEquivalentCharacter(x);
	}

	if(index < reg.size() && reg[index] == L'-') {
		index++;
		if(index < reg.size()) {
			if(reg[index] == L']') {
				index--;
				A.push_back(x);
			} else {
				wchar_t y = reg[index++];
				if(y == L'\\') {
					if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
					y = reg[index++];
					if(tolower(y) == L'd' || tolower(y) == L'w' || tolower(y) == L's') {
						throw RegexError(string("Invalid range: ") + unicode_to_utf8(x) + "-\\" + unicode_to_utf8(y));
					}
					y = getEquivalentCharacter(y);
				}
				if(y < x) {
					throw RegexError(string("Text range out of order: ") + unicode_to_utf8(x) + "-" + unicode_to_utf8(y));
				} else {
					for(int i = x; i <= y; i++) {
						A.push_back(i);
					}
				}
			}
		} else {
			throw RegexError("Unexpected end of regex encountered");
		}
	} else {
		A.push_back(x);
	}
	return A;
}

wchar_t NFA::getEquivalentCharacter(wchar_t c) {
	map<char, char> getEquivalent;
	getEquivalent['n'] = L'\n';
	getEquivalent['r'] = L'\r';
	getEquivalent['t'] = L'\t';
	getEquivalent['a'] = L'\a';
	getEquivalent['e'] = L'\e';
	getEquivalent['f'] = L'\f';
	getEquivalent['v'] = L'\v';
	getEquivalent['0'] = L'\0';

	if (getEquivalent.count(c))
		return getEquivalent[c];
	else return c;
}

vector<int> NFA::getPrebuildSet(wchar_t c) {
	set<int> S;
	vector<int> A;
	switch(c) {
	case L'd':
		for(int i = L'0'; i <= L'9'; i++)
			S.insert(i);
		break;
	case L'D':
		for(int i = 0; i < ALPHABET_SIZE; i++)
			S.insert(i);
		for(int i = L'0'; i <= L'9'; i++)
			S.erase(i);
		S.erase(EPSILON);
		break;
	case L'w':
		for(int i = L'0'; i <= L'9'; i++)
			S.insert(i);
		for(int i = L'A'; i <= L'Z'; i++)
			S.insert(i);
		for(int i = L'a'; i <= L'z'; i++)
			S.insert(i);
		S.insert(L'_');
		break;
	case L'W':
		for(int i = 0; i < ALPHABET_SIZE; i++)
			S.insert(i);
		for(int i = L'0'; i <= L'9'; i++)
			S.erase(i);
		for(int i = L'A'; i <= L'Z'; i++)
			S.erase(i);
		for(int i = L'a'; i <= L'z'; i++)
			S.erase(i);
		S.erase(L'_');
		S.erase(EPSILON);
		break;
	case L's':
		S.insert(L'\r');
		S.insert(L'\n');
		S.insert(L'\t');
		S.insert(L'\f');
		S.insert(L' ');
		break;
	case L'S':
		for(int i = 0; i < ALPHABET_SIZE; i++)
			S.insert(i);
		S.erase(L'\r');
		S.erase(L'\n');
		S.erase(L'\t');
		S.erase(L'\f');
		S.erase(L' ');
		S.erase(EPSILON);
		break;
	}

	for(set<int>::iterator i = S.begin(); i != S.end(); i++)
		A.push_back(*i);
	return A;
}

pair<int, int> NFA::parseChar(const wstring& reg, int& index) {
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));

	wchar_t x = reg[index++];
	if(x == L'+' || x == L'*' || x == L'?') {
		throw RegexError(string("Unexpected: ") + unicode_to_utf8(x));
	}
	if(x == L'\\') {
		if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
		x = reg[index++];
		if(tolower(x) == L'd' || tolower(x) == L'w' || tolower(x) == L's') {
			vector<int> symbols = getPrebuildSet(x);
			for(int i = 0; i < symbols.size(); i++) {
				transitions[startIndex][symbols[i]].insert(endIndex);
			}
		}
		x = getEquivalentCharacter(x);
	}
	transitions[startIndex][x].insert(endIndex);
	return make_pair(startIndex, endIndex);
}




///////////////////// external functions
std::wstring utf8_to_wstring(const std::string& str)
{
    return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}

std::string wstring_to_utf8(const std::wstring& str)
{
    return boost::locale::conv::utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
}

std::string unicode_to_utf8(uint32_t codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}
