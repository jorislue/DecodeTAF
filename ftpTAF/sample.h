#include <CkFtp2.h>
#include <CkString.h>

void ChilkatSample(void)
{
	CkFtp2 ftp;

	bool success;

	// Any string unlocks the component for the 1st 30-days.
	success = ftp.UnlockComponent("Anything for 30-day trial");
	if (success != true) {
		printf("%s\n", ftp.lastErrorText());
		return;
	}

	ftp.put_Hostname("www.example-code.com");
	ftp.put_Username("****");
	ftp.put_Password("****");

	// The default data transfer mode is "Active" as opposed to "Passive".

	// Connect and login to the FTP server.
	success = ftp.Connect();
	if (success != true) {
		printf("%s\n", ftp.lastErrorText());
		return;
	}

	// Change to the remote directory where the file is located.
	// This step is only necessary if the file is not in the root directory
	// for the FTP account.
	success = ftp.ChangeRemoteDir("junk");
	if (success != true) {
		printf("%s\n", ftp.lastErrorText());
		return;
	}

	// Download a file.
	CkString localFilename;
	localFilename = "hamlet.xml";
	CkString remoteFilename;
	remoteFilename = "hamlet.xml";

	success = ftp.GetFile(remoteFilename, localFilename);
	if (success != true) {
		printf("%s\n", ftp.lastErrorText());
		return;
	}

	ftp.Disconnect();

	printf("File Downloaded!\n");
}