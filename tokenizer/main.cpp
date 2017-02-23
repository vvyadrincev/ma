#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "TokenizerBuilder.h"
#include "Tokenizer.h"
using namespace std;


enum TokenType {
    ENDSENT,
    SEP,
    NUM,
    EN_BIG,
	EN_W,
	EN_LE,
	MAIL,
	RU_BIG,
	RU_W,
	RU_LE
};

int main(int argc, char const* argv[]) {
    std::string plaintext_path;
    if (argc < 2) {
        //std::cerr << "Invalid arguments. Usage:\n"
        //          << "\ttokenizer <plaintext>";
        //return 1;
        plaintext_path = "text_2.txt";
    }
    else
        plaintext_path = argv[1];


    //std::setlocale(LC_ALL, "ru_RU.UTF-8");
	try {
		TokenizerBuilder builder = TokenizerBuilder();
		builder.addTokenType(ENDSENT, L"[\\.\\?\\!\\;][ \\f\\r\\v\\n\\t]+");
		builder.addTokenType(SEP, L"[\\!\\?\\;\\.\\,\\)\\(\\*\\&\\^\\%\\$\\#\\@\\]\\[\\f\\r\\v\\n\\t]");
		builder.addTokenType(NUM, L"[0-9\\.,]*[0-9]+");
		builder.addTokenType(EN_BIG, L"[A-Z_\\-]+");
		builder.addTokenType(EN_W, L"[a-z_\\-]+");
		builder.addTokenType(EN_LE, L"[A-Z_\\-][a-z_\\-]*");
		builder.addTokenType(MAIL, L"[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+");
		builder.addTokenType(RU_BIG, L"[А-ЯЁ_\\-]+");
		builder.addTokenType(RU_W, L"[а-яё_\\-]+");
		builder.addTokenType(RU_LE, L"[А-ЯЁ\\-][а-яё_\\-]*");
		//builder.addTokenType()

		//Tokenizer tokenizer = builder.build();


        std::string line;
        std::ifstream ifs(plaintext_path);
        //std::string content( (std::istreambuf_iterator<char>(ifs) ),
        //                 (std::istreambuf_iterator<char>()    ) );
        //std::string con = "val21+3 - 25900/_x-MAX_SIZE";

        Tokenizer tokenizer = builder.build(true);
        //tokenizer.printDFA();
        //tokenizer.printRegexSymbols();

        if (ifs.is_open())
        {
            while (getline(ifs, line))
            {
                //std::string str((std::istreambuf_iterator<char>(ifs)),
                //                std::istreambuf_iterator<char>());
                //tokenizer = builder.build();

                //std::cout << "====== PARSING LINE: \n" << line << "\n========" << std::endl;
                std::wstring wstr = utf8_to_wstring(line);
                //wcout << wstr << L'\n';

                cout << line << endl;

                //wcout.clear();
                //std::cout << std::endl;
                    //std::cout << line << '\n';

                tokenizer.setInputStream(wstr);
                tokenizer.reset();

                while(tokenizer.hasNext()) {
                    Token token = tokenizer.nextToken();
                    //cout << token.getTokenType() << ": \""; wcout <<token.getLexeme(); cout << "\": " << token.getCharPosition() <<": " << token.getLineNumber() << "\n";
                    //cout << "abs charpos: " << token.getCharPosition() * token.getLineNumber();
                    cout << "Token: \"" << wstring_to_utf8(wstr.substr(token.getCharAbsPosition(), token.getLen()))
                         << "\", token type: " << token.getTokenType() << ", len: " << token.getLen()
                        << ", charpos: " << token.getCharPosition() <<", linenum: " << token.getLineNumber()
                         << ", abs charpos: " << token.getCharAbsPosition() << std::endl;
                }
            }
            ifs.close();
        }
        else
            std::cout << "Did not opened file " << plaintext_path << '\n';

    	//tokenizer.setInputStream(con);
		//tokenizer.setInputStream("val21+3 - 25900/_x-MAX_SIZE");

		//tokenizer.setInputStream(L"Синтаксический анализ является одним из базовых этапов для более сложных анализов текста. Открытых парсеров для русского языка днем с огнем не сыщешь. В этой статье я попытался заполнить этот пробел. При желании (и доступе к СинТагРус) вы можете обучить открытые парсеры и работать с русским языком. Точность обученного парсера не идеальная, хотя для многих задач она вполне приемлима. Однако, на мой взгляд, это хорошая стартовая площадка для улучшения результатов.");



	} catch(std::exception& e) {
		cout << e.what() << endl;
	}
	return 0;
}
