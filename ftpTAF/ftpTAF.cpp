
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

    int my_write(void *buffer, size_t size, size_t nmemb, void *param)
	{
		std::string& text = *static_cast<std::string*>(param);
		size_t totalsize = size * nmemb;
		text.append(static_cast<char*>(buffer), totalsize);
		return totalsize;
	}

	/*
	char* a und b stehen derzeit für den string mit dateiname
	*/

	__declspec(dllexport) char*__cdecl openFtpTAF(char* stationCode)
	{

		//Station Link: ftp://tgftp.nws.noaa.gov/data/observations/metar/stations/
		

		std::string result;
		string stringstation = stationCode;
		string fileending = ".TXT";
		string destURL = "ftp://tgftp.nws.noaa.gov/data/observations/metar/stations/" + stringstation + fileending;
		
		char* carrDestURL = const_cast<char*>(destURL.c_str());
		CURL *curl;
		CURLcode res;

		curl_global_init(CURL_GLOBAL_DEFAULT);

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, carrDestURL);
			/* Define our callback to get called when there's data to be written */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
			/* Set a pointer to our struct to pass to the callback */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

			/* Switch on full protocol/debug output */
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			res = curl_easy_perform(curl);
			cout << res;

			/* always cleanup */
			curl_easy_cleanup(curl);

			if (CURLE_OK != res) {
				/* we failed */
				std::cerr << "curl told us " << res << '\n';
			}
		}
			curl_global_cleanup();

			cout << result;
			char* CArrResult = const_cast<char*>(result.c_str());
			return CArrResult;
	}


#ifdef __cplusplus
}
#endif
