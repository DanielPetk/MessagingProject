#include "ServerNetworkController.h"

#include <algorithm>
#include <future>
#include <iostream>

#include <shared/protocol/ProtocolHandler.h>
#include <shared/shared.h>
#include "MainInterface.h"

constexpr int HOST_BUFFER_SIZE = 1024;

std::optional<std::string> ServerNetworkController::StartListening() {
    if (mRunning) {
        return std::nullopt;
    }
    mRunning = true;
    mServerSocket = Socket{AF_INET, SOCK_STREAM, IPPROTO_TCP};
  
    // get hostname
    std::string hostname(HOST_BUFFER_SIZE, '\0');
    gethostname(hostname.data(), HOST_BUFFER_SIZE - 1);

    // Set up socket server information
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  
    addr.sin_port = htons(mPort); 

    if (!mServerSocket.Bind(reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) {
        return std::nullopt;
        mRunning = false;
    }

    if (!mServerSocket.Listen(SOMAXCONN)) {
        return std::nullopt;
        mRunning = false;
    }

    AcceptClients();
    return hostname;
}

// Ugly
void ServerNetworkController::AcceptClients() {
    if (!mInterface || mLoopAccept) { return; }
    mLoopAccept = true;
    mAcceptThread = std::jthread{[&] {
        
        sockaddr_in clientSockAddr;
        socklen_t clientSockAddrSize = sizeof(clientSockAddr);
        ProtocolHandler handler;

        while (mLoopAccept) {
            auto clientOpt = mServerSocket.Accept(reinterpret_cast<sockaddr*>(&clientSockAddr), &clientSockAddrSize);
            if (!clientOpt) {continue;}

            Socket clientSocket = std::move(clientOpt.value());

            std::future<std::string> recvFuture = std::async(std::launch::async, [&]() {
                auto recvRes = clientSocket.Recv();
                if (!recvRes) return std::string{};
                return recvRes.value();
            });

            if (recvFuture.wait_for(std::chrono::milliseconds(2500)) == std::future_status::ready) {
                auto parsed = handler.ParseProtocolString(recvFuture.get());
                std::expected<int, int> sendRes;
                if (((parsed.contains(TYPE)) && (parsed[TYPE] == VALIDATE) && (parsed.contains(MESSAGE)) && (parsed[MESSAGE] == APP_IDENTIFIER) && (parsed.contains(USERNAME)))) {
                    sendRes = clientSocket.Send(handler.CreateProtocolString({{TYPE, VALIDATE}, {MESSAGE, SERVER_CONNECTION_ACCEPTED}}));
                    if (!sendRes || sendRes.value() == 0) {
                        continue;
                    }

                    // Add to clients list here
                    {
                        std::lock_guard<std::mutex> a{mClientMutex};
                        mClients.emplace_back(std::make_unique<Client>(clientSocket, parsed[USERNAME], this));
                        mInterface->OnClientJoined(parsed[USERNAME]);
                    }


                }
                else {
                    sendRes = clientSocket.Send(handler.CreateProtocolString({{TYPE, VALIDATE}, {MESSAGE, SERVER_CONNECTION_DECLINED}}));
                }
            }
            else {
                // Timeout
                clientSocket.Close();
                recvFuture.wait();
            }
        }
    }};
}

void ServerNetworkController::StopServer() {
    std::lock_guard<std::mutex> b{mCleanupMutex};
    mShuttingDown = true;
    mLoopAccept = false;
    mServerSocket.Close();
    if (mAcceptThread.joinable()) { mAcceptThread.join(); }
    {
        std::lock_guard<std::mutex> a{mClientMutex};
        mClients.clear();
    }
    mRunning = false;
    mShuttingDown = false;
}

void Client::CloseConnection() {
    mRunning = false;
    mClientSocket.Close();
    if (mClientThread.joinable()) {
        mClientThread.join();
    }
}

void Client::SendMessageToThisClient(const std::string& message) {
    mClientSocket.Send(message);
}

void Client::SendMessageToOtherClients(const std::string& message) {
    auto& clientVector = mNetwork->GetClients();

    std::for_each(clientVector.begin(), clientVector.end(), [&](std::unique_ptr<Client>& client) {
        if (client.get() != this) {
            client->SendMessageToThisClient(message);
        }
    });
}

Client::Client(Socket& socket, const std::string& username, ServerNetworkController* network) 
            : mClientSocket{std::move(socket)}, mUsername{username}, mNetwork{network} 
{
    mClientThread = std::jthread{[this] {
        while (mRunning) {
            auto recvRes = mClientSocket.Recv();
            if (!recvRes || recvRes.value() == "") {
                break;
            }

            std::string message = recvRes.value();
            std::unique_lock<std::mutex> ul{mNetwork->GetClientsMutex(), std::try_to_lock};
            if (ul.owns_lock()) {
                SendMessageToOtherClients(message);
                continue;
            }

            if (mNetwork->GetShuttingDown()){
                break;
            }

            std::lock_guard<std::mutex> lg{mNetwork->GetClientsMutex()};
            SendMessageToOtherClients(message);
        }

        mClientSocket.Close();
        mRunning = false;
    }};
}

