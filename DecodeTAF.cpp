#include "DecodeTAF.h"
#include <windows.h> 
#include <algorithm>
//#include "DecodeDLL.h"
//http://msdn.microsoft.com/de-de/library/ms235636(v=vs.90).aspx


typedef void(__cdecl *MYPROC)(char*, char*);
typedef void(__cdecl *MYPROC2)(char*);

DecodeTAF::DecodeTAF(void)
{
	//This is a change test
}


DecodeTAF::~DecodeTAF(void)
{
}


string* DecodeTAF::search_icoa_code(string _code)
{

	//fstream datei("C:/Users/Christopher/Desktop/icao_codes.txt", ios::in);
	ifstream f("C:/Users/Christopher/Desktop/icao_codes.txt");
	string zeile;
	string file="";
	string codes[4045];
	int count = 0;
	int first = 0;
	while (!f.eof())
	{
		getline(f, zeile);
		if (first == 0)
			cout <<"   "<< zeile << endl;

		if (zeile.find(_code) != std::string::npos)
		{
			
			cout << count << ": " <<zeile << endl;
			codes[count] = zeile.substr(4,4);
			cout << "Code: " <<codes[count] << endl;
			count++;
		}
			
		file += zeile;
		first++;
	}
	//f >> zeile;
	//cout << file;
	for (int i = 0; i++; i < count)
		cout << codes[i] << endl;
	//string tmp_array[200];
	
	return codes;
}




void main(){

	
	DecodeTAF* _decode = new DecodeTAF();
	HINSTANCE hinstLib;
	MYPROC ProcAdd;
	MYPROC2 ProcFtp;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
	//Commit TEST
	const int max_num = 100;
	char name[max_num] = "";
	char path[max_num] = ":";
	char both[max_num] = "";
	string _input;
	string in;
	string* icode;
	int number;

	HINSTANCE fptLib = LoadLibrary(TEXT("ftpTAF.dll"));
	{
		MYPROC2 ProcFtp = (MYPROC2)GetProcAddress(fptLib, "openFtpTAF");

		// If the function address is valid, call the function.

		if (NULL != ProcFtp)
		{
			char* station = "A302";
			fRunTimeLinkSuccess = TRUE;
			(ProcFtp)(station);
		}
		// Free the DLL module.

		fFreeResult = FreeLibrary(fptLib);
	}

	while (1)
	{
		cout << "City: ";
		/*getline(cin,_input);

		if (_input!="")
		{
		if (_input == "City")
		{*/
		getline(cin, in);
		transform(in.begin(), in.end(), in.begin(), ::toupper);
		cout << in << endl;
		icode = _decode->search_icoa_code(in);
		cout << "Number: ";
		//cin >> number;
		for (number = 0; number++; number < 10)
		{
			cout << icode[number] << endl;
		}
				
			//}
		//}


	}
	cout << "Path: ";
	cin >> path;
	cout << "File: ";
	cin >> name;


	strcat_s(name, max_num, ".txt");
	/*strcat_s(path, max_num, name);
	*/
	cout << "Path: " << path << endl;
	cout << "File: " << name << endl;

	hinstLib = LoadLibrary(TEXT("LocalTAF.dll"));



	if (hinstLib != NULL)
	{
		ProcAdd = (MYPROC)GetProcAddress(hinstLib, "openLocalTAF");

		// If the function address is valid, call the function.

		if (NULL != ProcAdd)
		{
			fRunTimeLinkSuccess = TRUE;
			(ProcAdd)(path, name);
		}
		// Free the DLL module.

		fFreeResult = FreeLibrary(hinstLib);
	}


	// If unable to call the DLL function, use an alternative.
	if (!fRunTimeLinkSuccess)
		printf("Message printed from executable\n");

	cout<<"DecodeTAF Console Application"<<endl;
	
	system("pause");
}