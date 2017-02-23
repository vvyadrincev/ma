#include "Tokenizer.h"

Tokenizer::Tokenizer(const DFA& dfa) {
    init(dfa);
}

Tokenizer::Tokenizer(const DFA& dfa, map<int, map<int, int>> stateSymbPos) {
    init(dfa);
	this->dfa.setStateSymbPos(stateSymbPos);
	this->dfa.minimizeStates();
	this->dfa.print();
}

void Tokenizer::init(const DFA& dfa){
    this->dfa = dfa;
	lineNumbersCounter = 1;
	characterPositionCounter = 0;
	characterAbsPositionCounter = 0;
	prepared = false;
	finished = false;
}

Tokenizer::~Tokenizer() {

}

void Tokenizer::setInputStream(const wstring& wstream) {
	this->stream.setBuffer(wstream);
	this->stream.reset();
	this->reset();
}

void Tokenizer::reset(){
	this->lineNumbersCounter = 1;
	this->characterPositionCounter = 0;
	this->characterAbsPositionCounter = 0;
	this->prepared = false;
	this->finished = false;
}

bool Tokenizer::hasNext() {
	checkPrepared();
	return !finished;
}

void Tokenizer::prepareToken() {

	while(!finished) {
		wstring lexeme = L"";
		stack<int> statesStack;
		int q = dfa.getStartingState();

		int characterPos = characterPositionCounter;
		int lineNumber = lineNumbersCounter;
		int characterAbsPos = characterAbsPositionCounter;

		bool foundFinal = dfa.isFinalState(q);

		statesStack.push(q);

		while(!stream.isFinished() && !dfa.isNullState(q) && !hopeless.count(make_pair(q, stream.getCursor()))) {
            wchar_t current_symbol = stream.get();
            //std::cout << "HERE: " << characterPos << ',' << lineNumber << ":";
            //std::wcout << L'\"'<< x << L"\": "; std::cout <<x << '\n';

			q = dfa.delta(q, int(current_symbol));
			if(dfa.isFinalState(q)) {
				foundFinal = true;
				statesStack = stack<int>();
			}
			statesStack.push(q);

			lexeme += current_symbol;
		}
		//std::wcout << '\n';

		if(foundFinal) {
			q = statesStack.top();
			statesStack.pop();
			while(!dfa.isFinalState(q)) {
				hopeless.insert(make_pair(q, stream.getCursor()));
				stream.unget();
				lexeme.erase(lexeme.size() - 1);
				q = statesStack.top();
				statesStack.pop();
			}

			int id = dfa.getFinalStateId(q);

			for(int i = 0; i < lexeme.length(); i++) {
				if(lexeme[i] == L'\n') {
					lineNumbersCounter++;
					characterPositionCounter = 0;
				} else {
					characterPositionCounter++;
				}
				characterAbsPositionCounter++;
			}

			//token = Token(id, lexeme, lineNumber, characterPos);
			//token = Token(id, lexeme, lexeme.size(), lineNumber, characterPos);
			token = Token(id, lexeme, lexeme.size(), lineNumber, characterPos, characterAbsPos);
			return;
		} else {
			if(stream.isFinished() && lexeme.length() == 0) {
				finished = true;
			} else {
				while(lexeme.size() != 0) {
					if(!statesStack.empty()) {
						q = statesStack.top();
						statesStack.pop();
						hopeless.insert(make_pair(q, stream.getCursor()));
					}
					stream.unget();
					lexeme.erase(lexeme.size() - 1);
				}
				wchar_t c = stream.get();
				if(c == L'\n') {
					lineNumbersCounter++;
					characterPositionCounter = 0;
				} else {
					characterPositionCounter++;
				}
				characterAbsPositionCounter++;
			}
		}
	}

	finished = true;
}

void Tokenizer::checkPrepared() {
	if(!prepared) {
		prepareToken();
		prepared = true;
	}
}

Token Tokenizer::nextToken() {
	checkPrepared();
	Token result = token;
	prepareToken();
	return result;
}

void Tokenizer::printDFA() {
     dfa.print();
}

