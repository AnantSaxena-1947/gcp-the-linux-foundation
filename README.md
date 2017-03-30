# gcp-the-linux-foundation


## Build Instructions

* You need 'libcurl'
* To install libcurl do ``` apt-get install libcurl4-gnutls-dev ```
* You can also install libcurl by ``` apt-get install libcurl4-openssl-dev ```

### Build from terminal
* Go to gcp directory
* do g++ utils.h gcp.cpp main.cpp -lcurl

### Build using eclipse
* If you have eclipse for C++, you can directly import this code.
* After importing, right click on the project, then go to Properties
* Select C/C++ Build, then go to Settings.
* Under GCC C++ Linker section on the right side, click on Libraries.
* Under libraries section click on '+' icon.
* Give 'curl' as the library name and then click on OK.
* Click Apply.
* Done!
