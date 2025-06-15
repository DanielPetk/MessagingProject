#include <winsock2.h>
#include <ws2tcpip.h> 
#include <windows.h>
#include <iostream>
#include "MainUI.h"


int main() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cerr << "Failed to init wsaData";
        std::exit(1);
    }

    MainUI mainUI;
    mainUI.Run();
}