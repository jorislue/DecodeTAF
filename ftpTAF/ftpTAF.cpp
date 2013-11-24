

//#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <curl.h>
using namespace std;

#define EOF (-1)
#include <fstream>
#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

	/*
	char* a und b stehen derzeit für den string mit dateiname
	*/
	__declspec(dllexport) int __cdecl openFtpTAF(char* a, char* b)
	{
		DWORD cchWritten;
		HANDLE hConout;
		BOOL fRet;
		const int numb = 100;
		char kilo[numb] = "";
		int max_char = 100;


		cout << "DLL!!" << endl;
		strcat_s(a, numb, b);
		cout << a << endl;
		fstream datei(a, ios::in);
		char zeile[200];
		while (datei.getline(zeile, 200))
		{
			cout << zeile << endl;

		}


		return 1;
	}

#ifdef __cplusplus
}
#endif
