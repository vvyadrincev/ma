#ifndef STRINGSTREAM_H_
#define STRINGSTREAM_H_

#include <string>
#include <iostream>

using namespace std;

class StringStream {
private:
	int cursor;
	wstring buffer;
public:
	StringStream();
	StringStream(const wstring& buffer);

	void reset();
	void setBuffer(const wstring& buffer);

	wchar_t get();
	wchar_t peek();
	bool isFinished() const;
	void unget();

	int getCursor() const;
};

#endif /* STRINGSTREAM_H_ */
