#include <iostream>

#include "utf-8/source/utf8.h"


int main()
{
    //setlocale(LC_ALL, "");
    std::cout << "Input the text (line): ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Inputed the text: " << name << std::endl;
    std::cout << "This string is " << name.size() << " bytes" << std::endl;
    
    std::string s = name;
    std::string line = s;
    
	std::string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
	if (end_it != line.end()) {
		std::cout << "Invalid UTF-8 encoding detected at text: " << line << "\n";
		std::cout << "This part is fine: " << std::string(line.begin(), end_it) << "\n";
	}
    
    
    int i;
	int letters = 0; // letters
	int numbers = 0; // numerical characters
	int other = 0; // other characters
	int probel = 0; // other characters
	int total; // total of all characters

	//i = 0;
	for (int i=0; i<s.size(); i++)
	    if (( s[i] >= 'a' && s[i] <= 'z') || (s[i] <= 'A' && s[i] >= 'Z'))
	    {
			letters++;
			i++;
		}
	////////////////////////////////////////////////////////////////////
	//count numerical characters
	    else
			if (( s[i] >= '0' && s[i] <= '9'))
			{
				numbers++;
			}
		else
			if (s[i] == ' ')
			{
				probel++;
			}
	////////////////////////////////////////////////////////////////////
	//count other characters
		else
	        {
				other++;
			}

	////////////////////////////////////////////////////////////////////
	//calculate total of characters
	////////////////////////////////////////////////////////////////////

	total = letters + numbers + other;

	////////////////////////////////////////////////////////////////////
	//output the results
	////////////////////////////////////////////////////////////////////

	std::cout << "Your string has " << total << " total characters" << std::endl;
	std::cout << letters << " letters " << std::endl;
	std::cout << numbers << " numerical characters " << std::endl;
	std::cout << probel << " probel characters " << std::endl;
	std::cout << other << " other characters " << std::endl;
    
    return 0;
}
//佐藤
