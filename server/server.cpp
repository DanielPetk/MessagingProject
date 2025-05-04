#include <winsock2.h>
#include <ws2tcpip.h> 
#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>

constexpr int HOST_BUFFER_SIZE = 1024;
constexpr int PORT_NUMBER = 54321;
constexpr int SEND_RECV_BUFFER_SIZE = 4096;

void cleanup(std::string message){
    std::cout << message;    
    WSACleanup();
}

void c1toc2(SOCKET c1sock, SOCKET c2sock);
void c2toc1(SOCKET c1sock, SOCKET c2sock);

int main(){

    // Initialize WSADATA
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cout << "Failed to init wsaData";
        return 1;
    }

    // Create a socket
    SOCKET ssock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ssock == INVALID_SOCKET){
        cleanup("Failed to initialize socket");
        return 1;
    }

    // Output hostname
    char hostname[HOST_BUFFER_SIZE];
    gethostname(hostname, HOST_BUFFER_SIZE);
    hostname[HOST_BUFFER_SIZE] = '\0';
    std::cout << "Hostname: " << hostname << std::endl;

    // Set up socket server information
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; 
    addr.sin_port = htons(PORT_NUMBER); // Example port
    std::cout << "Port Number: " << PORT_NUMBER << std::endl;

    // Bind the socket to a port
    if (bind(ssock, (sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(ssock);
        cleanup("Failed to bind socket to port");
        return 1;
    }

    // Listen for the first client.
    if (listen(ssock, 2) == SOCKET_ERROR){
        closesocket(ssock);
        cleanup("Failed to listen for first client");
        return 1;
    }

    // Set up client 1 socket informaiton
    sockaddr_in client1_addr;
    socklen_t c1len = sizeof(client1_addr);
   
    // Accept client 1 connection
    SOCKET c1sock = accept(ssock, (sockaddr*) &client1_addr, &c1len);
    if(c1sock == INVALID_SOCKET){
        closesocket(ssock);
        cleanup("Failed to accept first client connection");
        return 1;
    }
    std::cout << "First client connected!" << std::endl;

    // Listen for the second client.
    if (listen(ssock, 2) == SOCKET_ERROR){
        closesocket(ssock);
        closesocket(c1sock);
        cleanup("Failed to listen for second client");
        return 1;
    }

    // Set up client 2 socket informaiton
    sockaddr_in client2_addr;
    socklen_t c2len = sizeof(client2_addr);
   
    // Accept client 2 connection
    SOCKET c2sock = accept(ssock, (sockaddr*) &client2_addr, &c2len);
    if(c2sock == INVALID_SOCKET){
        closesocket(ssock);
        closesocket(c1sock);
        cleanup("Failed to accept second client connection");
        return 1;
    }
    std::cout << "Second client connected!" << std::endl;


    // Initialize the chat
    std::thread t1(c1toc2, c1sock, c2sock);
    std::thread t2(c2toc1, c1sock, c2sock);

    t1.join();
    t2.join();


    closesocket(c1sock);
    closesocket(c2sock);
    closesocket(ssock);
    cleanup("");
}

void c1toc2(SOCKET c1sock, SOCKET c2sock){
    char buffer[SEND_RECV_BUFFER_SIZE];
    while (true)
    {
        int bytes = recv(c1sock, buffer, SEND_RECV_BUFFER_SIZE, 0);
        if (bytes <= 0){
            return;
        }
        
        buffer[bytes] = '\0';
        std::cout << buffer;

        bytes = send(c2sock, buffer, bytes, 0);
        if (bytes < 0){
            return;
        }
    }
}

void c2toc1(SOCKET c1sock, SOCKET c2sock){
    char buffer[SEND_RECV_BUFFER_SIZE];
    while (true)
    {
        int bytes = recv(c2sock, buffer, SEND_RECV_BUFFER_SIZE, 0);
        if (bytes <= 0){
            return;
        }
        bytes = send(c1sock, buffer, bytes, 0);
        if (bytes < 0){
            return;
        }
    }
    
}