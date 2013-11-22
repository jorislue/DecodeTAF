#include "DecodeTAF.h"
#include <iostream>

#include <windows.h> 
//#include "DecodeDLL.h"
//http://msdn.microsoft.com/de-de/library/ms235636(v=vs.90).aspx
using namespace std;

typedef void(__cdecl *MYPROC)(char*, char*);

DecodeTAF::DecodeTAF(void)
{
	//This is a change test
}


DecodeTAF::~DecodeTAF(void)
{
}

void main(){

	HINSTANCE hinstLib;
	MYPROC ProcAdd;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

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

	cout<<"DecodeTAF Console Application"<<endl;
	
	system("pause");
}