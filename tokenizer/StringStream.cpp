#include "StringStream.h"

//#include "../../../utf8_v2_3_4/source/utf8.h"



StringStream::StringStream() {
	cursor = 0;
}

StringStream::StringStream(const wstring& buffer) {
	cursor = 0;
	this->buffer = buffer;
}

void StringStream::reset() {
	cursor = 0;
}

void StringStream::setBuffer(const wstring& buffer) {
	this->buffer = buffer;
}

wchar_t StringStream::get() {
    //cout<< "Getting symbol by cursor = " << cursor << ": " << buffer[cursor] << '\n' ; //wcout << &buffer[cursor] << L'\n';
	return buffer[cursor++];
}

wchar_t StringStream::peek() {
	return buffer[cursor];
}

bool StringStream::isFinished() const {
	return (cursor == buffer.size());
}

void StringStream::unget() {
	if(cursor > 0) cursor--;
}

int StringStream::getCursor() const {
	return cursor;
}
