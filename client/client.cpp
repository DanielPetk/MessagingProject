#include <iostream>

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h> 
    #include <windows.h>
#else
    #include <array>
#endif

#include "ClientApp.h"

int main() {

#if defined(_WIN32)
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cerr << "Failed to init wsaData";
        std::exit(1);
    }
#endif

    ClientApp clientApp;
    clientApp.Run();

#if defined(_WIN32)
    WSACleanup();
#endif
}