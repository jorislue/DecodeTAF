#pragma once
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
using namespace std;

class DecodeTAF
{
public:
	//Main changes
	DecodeTAF(void);
	~DecodeTAF(void);
	string* search_icoa_code(string);

};

