# BrowseSpy

Be sure to change the ftp variables throughout the code, these variables contain the username, password, & IP address of the FTP server which receives the files.

This code will do the following:

1. Copy itself into the %TMP% directory & name itself ursakta.exe
2. Add a registry entry to execute itself each time the user logs in
3. Verify which browser the user is using (Chrome, Firefox or Brave)
4. Search for files within the Chrome, Firefox, or Brave browser directories
5. Create a directory on our FTP server then send the files in the browser's directory to the FTP server

# Cross Compiling with MingW on Linux

Install command with Apt:

* `sudo apt-get install mingw-w64`

64-bit:

* `x86_64-w64-mingw32-gcc *input file* -o *output file* -lwininet -lversion`

32-bit:

* `i686-w64-mingw32-gcc *input file* -o *output file* -lwininet -lversion`
# From Victim's Perspective:

Registry entry:

![](/imgs/img1.png)

File activity:

![](/imgs/img2.png)

![](/imgs/img3.png)

FTP connection:

![](/imgs/img4.png)

# Detection Rate:

This detection rate is after stripping the executable with `strip --strip-all *filename.c*`

![](/imgs/img5.png)
