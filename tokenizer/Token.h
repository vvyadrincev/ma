#ifndef TOKENOBJECT_H_
#define TOKENOBJECT_H_
#include <string>

using namespace std;

class Token {
private:
	int tokenType;
	wstring lexeme;
	int len;
	int lineNumber;
	int charPosition;
	int charAbsPosition;
public:
	Token();
	Token(int tokenType);
	Token(int tokenType, const wstring& lexeme);
	Token(int tokenType, const wstring& lexeme, int lineNumber, int charPosition);
	Token(int tokenType, const wstring& lexeme, int len, int lineNumber, int charPosition);
	Token(int tokenType, const wstring& lexeme, int len, int lineNumber, int charPosition, int charAbsPosition);
	virtual ~Token();

	int getCharPosition() const;
	void setCharPosition(int charPosition);

	int getCharAbsPosition() const;
	void setCharAbsPosition(int charAbsPosition);

	wstring getLexeme() const;
	void setLexeme(wstring lexeme);

    int getLen() const;
    void setLen(int len);

	int getLineNumber() const;
	void setLineNumber(int lineNumber);

	int getTokenType() const;
	void setTokenType(int tokenType);
};

#endif
