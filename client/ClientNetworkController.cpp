#include <future>
#include <chrono>

#include "ClientNetworkController.h"
#include <shared/shared.h>

bool ClientNetworkController::ConnectToServer(const std::string& username, const std::string& host, const std::string& port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(port));
    
    hostent* he = gethostbyname(host.c_str());
    if (!he) {
        return false;
    }
    memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

    // Create socket
    mServerSocket = Socket( AF_INET, SOCK_STREAM, 0 );
    // Connect to server 
    mServerSocket.Connect(reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

    return ValidateServer(username);
}

bool ClientNetworkController::ValidateServer(const std::string& username) {
    std::string validationMessage = APP_IDENTIFIER + DELIM + username;
    auto sendRes = mServerSocket.Send(validationMessage, 0);

    // If the socket is blocked (should never happen) or if the other side of the connection is not closed
    if (!sendRes || sendRes.value() == 0) {
        mServerSocket.Close();
        return false;
    }

    std::future<std::string> recvFuture = std::async(std::launch::async, [this]() {
        auto recvRes = mServerSocket.Recv();
        if (!recvRes) return std::string{};
        return recvRes.value();
    });

    if (recvFuture.wait_for(std::chrono::seconds(1)) == std::future_status::ready) {
        return recvFuture.get() == SERVER_CONNECTION_ACCEPTED;
    } else {
        // TODO THIS WILL CAUSE ERROR PLEASE FIX
        mServerSocket.Close();
        recvFuture.wait();
        return false;
    }
}