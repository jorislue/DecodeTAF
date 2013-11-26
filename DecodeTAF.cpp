#include "DecodeTAF.h"
#include <windows.h> 
#include <algorithm>
//#include "DecodeDLL.h"
//http://msdn.microsoft.com/de-de/library/ms235636(v=vs.90).aspx


typedef void(__cdecl *MYPROC)(char*, char*);
typedef char* (__cdecl *MYPROC2)(char*,char*);
static char reportData[150];

DecodeTAF::DecodeTAF(void)
{
	//This is a change test
}


DecodeTAF::~DecodeTAF(void)
{
}

void DecodeTAF::setlocalpath(string _path)
{
	this->localpath = _path;
}

void DecodeTAF::search_icoa_code()
{
	for (int i = 0; i < 4045; i++)
		_codes[i] = "";

	cout << "Please Type in the airport or city for a weather report.\n";
	cout << "City: ";
	getline(cin, _city);
	
	transform(_city.begin(), _city.end(), _city.begin(), ::toupper);
	cout << _city << endl;

	//fstream datei("C:/Users/Christopher/Desktop/icao_codes.txt", ios::in);
	ifstream f("C:/Users/Christopher/Desktop/icao_codes.txt");
	string zeile;
	string file="";
	int number=0;
	int count = 0;
	int first = 0;
	while (!f.eof())
	{
		getline(f, zeile);
		if (first == 0)
			cout <<"   "<< zeile << endl;

		if (zeile.find(_city) != std::string::npos)
		{			
			cout << count << ": " <<zeile << endl;
			_codes[count] = zeile.substr(4,4);
			cout << "Code: " <<_codes[count] << endl;
			count++;
		}
			
		file += zeile;
		first++;
	}
	

	while ((cout << "Number? ")
		&& (!(cin >> number) || number < 0 || number > 4046)) {
			cout << "That's not a number between 0 and 4046; ";
			cin.clear();
			cin.ignore();
	}

	cin.ignore();
	cout << callFromFTP(_city);
	
	cout << "Inhalt: " << _codes[number] << endl;
	  _codes[number];
}
string callFromFTP(string stationcode);



void main(){

	string _method = "";
	DecodeTAF* _decode = new DecodeTAF();
	string _input;
	string in;
	string icode;
	string path;
	string _option = "";
	int number;
	
	while (1)
	{
		
		cout << "Enter ftp or local " << endl;
		cin >> _method;
		transform(_method.begin(), _method.end(), _method.begin(), ::tolower);
		cout << _method << endl;
		cin.ignore();
		if (_method == "ftp")
		{
		
			cout << "FTP call" << endl;
		
		}
		else if (_method == "local")
		{
			cout << "With path you can enter your path to the file." << endl << "With icao you can directly enter the ICAO-code and the file will be opened." << endl << "With search you can search for a specific airport" << endl;
			cin >> _option;
			transform(_option.begin(), _option.end(), _option.begin(), ::tolower);
			if (_option == "path")
			{
				cin >> path;
				_decode->setlocalpath(path);
			}
			else if (_option == "icao")
			{
				
			}
			else if (_option == "search")
			{
				cin.ignore();
				_decode->search_icoa_code();
			}


			
			cout << "Code in main: " << icode << endl;

			cout << "Download latest forecast via FTP? (default)" << endl;
			
		}
		else
		{
			cout << "Wrong Input" << endl;
		}
		

		
	}

	system("pause");
}



string DecodeTAF::callFromFTP(string stationcode){

	MYPROC2 ProcFtp;
	string report;
	BOOL fFreeResult,fRunTimeLinkSuccess = false;
	HINSTANCE fptLib = LoadLibrary(TEXT("ftpTAF.dll"));
	{
		ProcFtp = (MYPROC2)GetProcAddress(fptLib, "openFtpTAF");

		// If the function address is valid, call the function.

		if (NULL != ProcFtp)
		{
			char *station = const_cast<char*>(stationcode.c_str());;
			fRunTimeLinkSuccess = TRUE;
			report = string((ProcFtp)(station,reportData));
		}
		// Free the DLL module.

		fFreeResult = FreeLibrary(fptLib);
	}
	cout << report;
	return report;

}

string callFromLocalFile(){

	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
	MYPROC ProcAdd;
	HINSTANCE hinstLib;
	const int max_num = 100;
	char name[max_num] = "";
	char path[max_num] = ":";
	char both[max_num] = "";

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

	cout << "DecodeTAF Console Application" << endl;
	return NULL;
}