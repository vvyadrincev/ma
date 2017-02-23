#include "Token.h"

Token::Token() {
	this->tokenType = -1;
	this->lexeme = L"";
	this->lineNumber = 0;
	this->charPosition = 0;
	this->len = 0;
	this->charAbsPosition = 0;
}

Token::Token(int tokenType) {
	this->tokenType = tokenType;
	this->lexeme = L"";
	this->lineNumber = 0;
	this->charPosition = 0;
	this->len = 0;
	this->charAbsPosition = 0;
}

Token::Token(int tokenType, const wstring& lexeme) {
	this->tokenType = tokenType;
	this->lexeme = lexeme;
	this->lineNumber = 0;
	this->charPosition = 0;
	this->len = lexeme.size();
	this->charAbsPosition = 0;
}

Token::Token(int tokenType, const wstring& lexeme, int lineNumber, int charPosition) {
	this->tokenType = tokenType;
	this->lexeme = lexeme;
	this->lineNumber = lineNumber;
	this->charPosition = charPosition;
	this->len = lexeme.size();
	this->charAbsPosition = 0;
}

Token::Token(int tokenType, const wstring& lexeme, int len, int lineNumber, int charPosition, int charAbsPosition) {
	this->tokenType = tokenType;
	this->lexeme = lexeme;
	this->len = len;
	this->lineNumber = lineNumber;
	this->charPosition = charPosition;
	this->charAbsPosition = charAbsPosition;
}

Token::~Token() {

}

int Token::getCharPosition() const {
	return charPosition;
}

void Token::setCharPosition(int charPosition) {
	this->charPosition = charPosition;
}

int Token::getCharAbsPosition() const {
	return charAbsPosition;
}

void Token::setCharAbsPosition(int charAbsPosition) {
	this->charAbsPosition = charAbsPosition;
}

wstring Token::getLexeme() const {
	return lexeme;
}

void Token::setLexeme(wstring lexeme) {
	this->lexeme = lexeme;
}

int Token::getLen() const {
	return len;
}

void Token::setLen(int len) {
	this->len = len;
}

int Token::getLineNumber() const {
	return lineNumber;
}

void Token::setLineNumber(int lineNumber) {
	this->lineNumber = lineNumber;
}

int Token::getTokenType() const {
	return tokenType;
}

void Token::setTokenType(int tokenType) {
	this->tokenType = tokenType;
}
