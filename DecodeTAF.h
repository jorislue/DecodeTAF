#pragma once
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <limits>
using namespace std;

class DecodeTAF
{
public:
	//Main changes
	DecodeTAF(void);
	~DecodeTAF(void);
	void search_icoa_code();
	void setlocalpath(string);
	void setftppath(string);
	string getlocalpath();
	string getftppath();
	string callFromLocalFile();
	string callFromFTP(string);
private:
	string _codes[4045];
	string _city;
	string localpath;
};

