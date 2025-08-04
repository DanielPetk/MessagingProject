#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <windows.h>

#include "ClientApp.h"

int main() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cerr << "Failed to init wsaData";
        std::exit(1);
    }

    ClientApp clientApp;
    clientApp.Run();

    WSACleanup();
}