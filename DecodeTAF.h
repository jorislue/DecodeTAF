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
	void search_icoa_code(int);
	void setlocalpath(string);
	void setftppath(string);
	string getlocalpath();
	string getftppath();
	void callFromLocalFile(string);
	void callFromFTP(string);
	void setlocal_file(string);
	string getlocal_file();
private:
	string local_file;
	string _city;
	string localpath;
	char* localfile;
	string result_local;
	string _localfile;
	
	
};

