#include "DecodeTAF.h"
#include <windows.h> 
#include <algorithm>
#include "TAFDecoder.h"

//Declaration of DLL-Functions
typedef char*(__cdecl *MYPROC)(char*,char*);
typedef char* (__cdecl *MYPROC2)(char*,char*);
//Static result Array for extern call
static char reportData[150];
//String-Array with aviable ICAO Codes
string _codes[4045] = { "" };

//Sets the local path for the ICAO Airport List
DecodeTAF::DecodeTAF(void)
{
	this->localpath = "C:\\Users\\esKay\\Dropbox\\FH Master\\Spezielle Softwaretechnologien\\Decode TAF\\";
}

//Destructor
DecodeTAF::~DecodeTAF(void)
{
}

//Saves the content of the specified textfile
void DecodeTAF::setlocal_file(string _result)
{
	this->result_local = _result;
}
//Returns the Content (getter)
string DecodeTAF::getlocal_file()
{
	return this->result_local;
}

//Sets the Path for the local textfile
void DecodeTAF::setlocalpath(string _path)
{
	this->localpath = _path;
}

//getter of local path
string DecodeTAF::getlocalpath()
{
	return this->localpath;
}

//Searches for ICAO-Codes, variation -> Ftp or Local call after search
void DecodeTAF::search_icoa_code(int _variation)
{
	for (int i = 0; i < 4045; i++)
		_codes[i] = "";

	cout << "Please Type in the airport or city for a weather report.\n";
	cout << "City: ";
	getline(cin, _city);
	transform(_city.begin(), _city.end(), _city.begin(), ::toupper);
	
	ifstream f("C:\\Users\\esKay\\Dropbox\\FH Master\\Spezielle Softwaretechnologien\\Decode TAF\\icao_codes.txt");
	if (!f.fail())
	{
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
			count++;
		}

		file += zeile;
		first++;
	}


	while ((cout << "Please specify the line-number of your airport: ")
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
	else
	{
		cout << "File not found or could not be opened." << endl;
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
		
		cout << "Type \"ftp\" for an FTP call or \"local\" if you want\n to decode a file on your system." << endl;
		cin >> _method;
		transform(_method.begin(), _method.end(), _method.begin(), ::tolower);
		cin.ignore();

		if (_method == "ftp")
		{
			_number = 1;
			cout << "\nFTP:" <<endl <<"With typing \"icao\" you can directly enter the ICAO-code and the file will be opened." << endl << "When you type \"search\" you can search for a specific airport" << endl;
			cin >> _option;
			transform(_option.begin(), _option.end(), _option.begin(), ::tolower);

			if (_option == "icao")
			{
				cin.ignore();
				cout << "\nFile name: e.g LOWS" << endl;
				while (cin >> icao && icao.length() != 4)
				{
					cout << "Eingabe hat keine 4 zeichen! " << endl;
					cin.clear();
					cin.ignore();
				}
				_decode->callFromFTP(icao);
				if (_decode->getlocal_file()!="")
				cout << TAFDecoding::TheTAFDecoder::decode(_decode->getlocal_file(), "   ") << endl;
			}
			else if (_option == "search")
			{
				cin.ignore();
				_decode->search_icoa_code(_number);
				if (_decode->getlocal_file() != "")
				cout<<TAFDecoding::TheTAFDecoder::decode(_decode->getlocal_file(), "   ")<<endl;
			}
			else
			{
				cout << "Wrong input, going back..." << endl;
			}
			cout << "\n\n";
		
		}
		else if (_method == "local")
		{
			_number = 2;
			cout << "\nLocal:"<<endl<<"With \"localpath\" you can enter your path to the local files." << endl << "With \"icao\" you can directly enter the ICAO-code and the file will be opened." << endl << "With \"search\" you can search for a specific airport" << endl;
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
				cout << "\n";
				_decode->callFromLocalFile(icao);
				if (_decode->getlocal_file() != "")
				cout << TAFDecoding::TheTAFDecoder::decode(_decode->getlocal_file(), "   ") << endl;
			}
			else if (_option == "search")
			{
				cin.ignore();
				_decode->search_icoa_code(_number);
				if (_decode->getlocal_file() != "")
				cout << TAFDecoding::TheTAFDecoder::decode(_decode->getlocal_file(), "   ") << endl;
			}
			else
			{
				cout << "Wrong input, going back..." << endl;
			}
			cout << "\n\n";
			
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


//Loads the DLL for the FTP call, stationcode -> ICAO-Code
void DecodeTAF::callFromFTP(string stationcode){

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
	this->setlocal_file(report);

}
//Loads the DLL for the local call, file -> ICAO-Code
void DecodeTAF::callFromLocalFile(string file){

	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
	MYPROC ProcAdd;
	HINSTANCE hinstLib;
	const int max_num = 100;
	char _file[100] = { "" }; 
	char _localpath[100] = { "" };
	char result[600] = { "" };
	for (int j = 0; j < this->localpath.length(); j++)
		_localpath[j] = this->localpath[j];

	for (int k = 0; k < file.length(); k++)
		_file[k] = file[k];

	strcat_s(_file, max_num, ".txt");
	strcat_s(_localpath, 100, _file);
	
	cout << "\nSetted File / Path: " << _localpath << endl;

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
