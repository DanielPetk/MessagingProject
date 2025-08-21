#include <chrono>
#include <iostream>

#include "ClientNetworkController.h"
#include "MainInterface.h"
#include <shared/shared.h>


ClientNetworkController::~ClientNetworkController() {
    mServerSocket.Shutdown(SD_BOTH);
    std::cout << "Cleaning Up!";
    if (mConnectingFuture.valid()) { mConnectingFuture.get(); }
}

void ClientNetworkController::ConnectToServer(const std::string& username, const std::string& host, const std::string& port) {    
    mConnectingFuture = std::async(std::launch::async, [=, this] {
        if (!mInterface) { return; }
        
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(std::stoi(port));
        
        hostent* he = gethostbyname(host.c_str());
        if (!he) {
            mInterface->OnConnectionError();
            return;
        }
        memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

        // Create socket
        mServerSocket = Socket( AF_INET, SOCK_STREAM, 0 );

        // Connect to server 
        if (!mServerSocket.Connect(reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr))) {
            mServerSocket.Shutdown(SD_BOTH);
            mInterface->OnConnectionError();
            return;
        }

        if(ValidateServer(username)) {
            mInterface->OnConnectionSuccess();
        }
        else {
            mInterface->OnConnectionError();
        }
    });

}

bool ClientNetworkController::ValidateServer(const std::string& username) {
    std::string validationMessage = APP_IDENTIFIER + DELIM + username;
    auto sendRes = mServerSocket.Send(validationMessage, 0);

    // If error or if the other side of the connection is closed
    if (!sendRes || sendRes.value() == 0) {
        mServerSocket.Shutdown(SD_BOTH);
        return false;
    }

    std::future<std::string> recvFuture = std::async(std::launch::async, [this]() {
        auto recvRes = mServerSocket.Recv();
        if (!recvRes) return std::string{};
        return recvRes.value();
    });

    if (recvFuture.wait_for(std::chrono::milliseconds(500)) == std::future_status::ready) {
        return recvFuture.get() == SERVER_CONNECTION_ACCEPTED;
    }

    mServerSocket.Shutdown(SD_BOTH);
    recvFuture.wait();
    return false;
}

void ClientNetworkController::AddInterface(std::shared_ptr<MainInterface> mainInterface) {
    mInterface = mainInterface;
}

