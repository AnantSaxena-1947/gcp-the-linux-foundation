# gcp-the-linux-foundation


## Build Instructions

### Build from terminal
* g++ utils.h gcp.cpp main.cpp -lcurl

### Build using eclipse
* If you have eclipse for C++, you can directly import this code.
* After importing, right click on the project, then go to Properties
* Select C/C++ Build, then go to Settings.
* Under GCC C++ Linker section on the right side, click on Libraries.
* Under libraries section click on '+' icon.
* Give 'curl' as the library name and then click on OK.
* Click Apply.
* Done!
