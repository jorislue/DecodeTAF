#include "DecodeTAF.h"
#include <windows.h> 
#include <algorithm>
#include "TAFDecoder.h"
//#include "DecodeDLL.h"
//http://msdn.microsoft.com/de-de/library/ms235636(v=vs.90).aspx


typedef char*(__cdecl *MYPROC)(char*,char*);
typedef char* (__cdecl *MYPROC2)(char*,char*);
static char reportData[150];
string _codes[4045] = { "" };

DecodeTAF::DecodeTAF(void)
{
	this->localpath = "C:/Users/Christopher/Downloads/";
}

DecodeTAF::~DecodeTAF(void)
{
}

void DecodeTAF::setlocal_file(string _result)
{
	this->result_local = _result;
}

string DecodeTAF::getlocal_file()
{
	return this->result_local;
}

void DecodeTAF::setlocalpath(string _path)
{
	this->localpath = _path;
}

string DecodeTAF::getlocalpath()
{
	return this->localpath;
}

void DecodeTAF::search_icoa_code(int _variation)
{
	for (int i = 0; i < 4045; i++)
		_codes[i] = "";

	cout << "Please Type in the airport or city for a weather report.\n";
	cout << "City: ";
	getline(cin, _city);
	transform(_city.begin(), _city.end(), _city.begin(), ::toupper);
	
	ifstream f("C:/Users/Christopher/Desktop/icao_codes.txt");
	string zeile;
	string file = "";
	int number = 0;
	int count = 0;
	int first = 0;
	while (!f.eof())
	{
		getline(f, zeile);
		if (first == 0)
			cout << "   " << zeile << endl;

		if (zeile.find(_city) != std::string::npos)
		{
			cout << count << ": " << zeile << endl;
			_codes[count] = zeile.substr(4, 4);
			cout << "Code: " << _codes[count] << endl;
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

	if (_codes[number] == "")
	{
		cout << "No airport chosen!" << endl;
	}
	else
	{
		if (_variation == 1)
		{
			callFromFTP(_codes[number]);
		}
		else if (_variation == 2)
		{
			callFromLocalFile(_codes[number]);
		}
		else
		{
			cout << "No method choosen" << endl;
		}
	}
}




void main(){

	string _method = "";
	DecodeTAF* _decode = new DecodeTAF();
	string _input;
	string path;
	string _option = "";
	int _number;
	string icao;
	while (1)
	{
		
		cout << "Enter ftp or local " << endl;
		cin >> _method;
		transform(_method.begin(), _method.end(), _method.begin(), ::tolower);
		cout << _method << endl;
		cin.ignore();

		if (_method == "ftp")
		{
			_number = 1;
			cout << "FTP:" <<endl <<"With icao you can directly enter the ICAO-code and the file will be opened." << endl << "With search you can search for a specific airport" << endl;
			cin >> _option;
			transform(_option.begin(), _option.end(), _option.begin(), ::tolower);
			
			/*string _stationcode = "";
			cout << "FTP call" << endl;
			cout << "Station Code" << endl;
			cin >> _stationcode;
			string test = _decode->callFromFTP(_stationcode);
			cout << "\nRaw report:\n\n";
			cout << test;
			cout << "\n\n";*/
			if (_option == "icao")
			{
				cin.ignore();
				cout << "File name: e.g LOWS" << endl;
				while (cin >> icao && icao.length() != 4)
				{
					cout << "Eingabe hat keine 4 zeichen! " << endl;
					cin.clear();
					cin.ignore();
				}

				_decode->callFromFTP(icao);
			}
			else if (_option == "search")
			{
				cin.ignore();
				_decode->search_icoa_code(_number);
			}
		
		}
		else if (_method == "local")
		{
			_number = 2;
			cout << "Local:"<<endl<<"With localpath you can enter your path to the local files." << endl << "With icao you can directly enter the ICAO-code and the file will be opened." << endl << "With search you can search for a specific airport" << endl;
			cin >> _option;
			transform(_option.begin(), _option.end(), _option.begin(), ::tolower);
			if (_option == "localpath")
			{
				cin.ignore();
				cin >> path;
				_decode->setlocalpath(path);
			}
			else if (_option == "icao")
			{
				cin.ignore();
				cout << "File name: e.g LOWS" << endl;
				while (cin >> icao && icao.length() != 4)
				{
					cout << "Eingabe hat keine 4 zeichen! " << endl;
					cin.clear();
					cin.ignore();
				}

				_decode->callFromLocalFile(icao);
			}
			else if (_option == "search")
			{
				cin.ignore();
				_decode->search_icoa_code(_number);
			}

			cout << "Result in main(): " << _decode->getlocal_file();
		
			
		}
		else
		{
			cout << "Wrong Input" << endl;
		}
		
		_number = 0;
		_option = "";
		_method = "";
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

		
		if (NULL != ProcFtp)
		{
			char *station = const_cast<char*>(stationcode.c_str());;
			fRunTimeLinkSuccess = TRUE;
			report = string((ProcFtp)(station,reportData));
		}

		fFreeResult = FreeLibrary(fptLib);
	}
	cout << "In callfromftp: " << report;
	return report;

}

void DecodeTAF::callFromLocalFile(string file){

	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
	MYPROC ProcAdd;
	HINSTANCE hinstLib;
	const int max_num = 100;
	char _file[100] = { "" }; 
	char _localpath[100] = { "" };
	char result[600] = { "" };
	cout << "Erstes File " << file << endl;
	for (int j = 0; j < this->localpath.length(); j++)
		_localpath[j] = this->localpath[j];

	cout << "Path: " << _localpath << endl;


	for (int k = 0; k < file.length(); k++)
		_file[k] = file[k];

	cout << "File: " << _file << endl;

	strcat_s(_file, max_num, ".txt");
	strcat_s(_localpath, 100, _file);
	
	cout << "Both: " << _localpath << endl;
	
	system("pause");

	hinstLib = LoadLibrary(TEXT("LocalTAF.dll"));



	if (hinstLib != NULL)
	{
		ProcAdd = (MYPROC)GetProcAddress(hinstLib, "openLocalTAF");

		// If the function address is valid, call the function.

		if (NULL != ProcAdd)
		{
			fRunTimeLinkSuccess = TRUE;
			this->localfile = ((ProcAdd)(_localpath,result));
		}
		// Free the DLL module.
		if (this->localfile != "")
		{

			this->_localfile = this->localfile;
			cout << "Result of dll: " << this->localfile << endl;
			this->setlocal_file(this->_localfile);
		}
		else
			this->setlocal_file("");

		fFreeResult = FreeLibrary(hinstLib);
	}


	// If unable to call the DLL function, use an alternative.
	if (!fRunTimeLinkSuccess)
		printf("Message printed from executable\n");

	
}
