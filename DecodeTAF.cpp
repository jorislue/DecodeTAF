#include "DecodeTAF.h"
#include <iostream>
#include "DecodeDLL.h"
//http://msdn.microsoft.com/de-de/library/ms235636(v=vs.90).aspx
using namespace std;

DecodeTAF::DecodeTAF(void)
{
	//This is a change test
}


DecodeTAF::~DecodeTAF(void)
{
}

void main(){
	cout<<"DecodeTAF Console Application"<<endl;
	cout<<returnDecodedTAF("bla");
	system("pause");
}