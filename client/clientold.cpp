#include <winsock2.h>
#include <ws2tcpip.h> 
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <chrono>
#include <list>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

using namespace ftxui;

constexpr int HOST_BUFFER_SIZE = 256;
constexpr int SEND_RECV_BUFFER_SIZE = 4096;
constexpr int PORT_NUMBER = 54321;
constexpr int MAX_QUEUE_SIZE = 20;
constexpr int FIRST_MSG_LINE = 4;

std::mutex sock_mtx; // Mutex for sockets
std::mutex stream_mtx; // Mutex for sockets
std::string input; // Global input state

// App states
enum State {InputForm, ConError, Chat};

int somestate = 0;
// Current state
State AppState = State::InputForm;

void cleanup(std::string message);
void addMsg(const std::string& msg, bool clear);
void receiveLoop(SOCKET sock);
void sendLoop(SOCKET sock);
std::string formatMsg(const std::string& msg, bool other);
void winInit();

SOCKET connectToServer(const std::string& host, int port);

int main(){

    winInit();

    auto screen = ScreenInteractive::FitComponent();

    std::string username, hostname, port;

    auto usernameField = Input(&username);
    usernameField |= CatchEvent([&](Event event) {
        if (event.is_character()) {
            if (event.character()[0] == ' ') {return true;}
            if (username.size() >= 16) {return true;}
        }
        return event == Event::Return; // Prevent newline
    });
    auto hostnameField = Input(&hostname);
    hostnameField |= CatchEvent([&](Event event) {
        return event == Event::Return;
    });
    auto portField = Input(&port);
    portField |= CatchEvent([&](Event event) {
        if (event.is_character()) {
            if (port.size() >= 5) { return true; } // max port size
            if (!std::isdigit(event.character()[0])) { return true; }
            int newport = std::stoi(port+event.character()[0]);
            return (newport < 0 || newport > 65535);
        }
        if (event == Event::Return) { return true; }
        return false;
    });
    auto connectButton = Button("Connect", [&] {
        SOCKET ssock = connectToServer(hostname, std::stoi(port));
        if (ssock == SOCKET_ERROR || ssock == INVALID_SOCKET){
            AppState = State::ConError;
            somestate = 1;
            return;
        }
    });
      
    auto inputContainer = Container::Vertical({
        usernameField,
        hostnameField,
        portField,
        connectButton
    });

    auto input = Renderer(inputContainer, [&] {
        return window(text("Connect To Server"),
            hbox({
            separatorEmpty(),
            vbox({
                separatorEmpty(),
                hbox({text("Username: "), usernameField->Render()}),
                hbox({text("Hostname: "), hostnameField->Render()}),
                hbox({text("Port: "), portField->Render() | size(WIDTH, EQUAL, 6)}), // 5 digits for port num
                separatorDashed(),
                hbox({filler(), connectButton->Render(), filler()})
            }) | size(WIDTH, EQUAL, 30), separatorEmpty()
        })
        );
    });

    auto errOkButton = Button("OK", [&] {
        AppState = State::InputForm;
        somestate = 0; 
        
    });

    auto err = Renderer(errOkButton, [&] {
        return window(text("Connection Error"),
        hbox({
            separatorEmpty(),
            vbox({
                separatorEmpty(),
                paragraph("There was an error connecting to the server."),
                separatorEmpty(),
                hbox({filler(), errOkButton->Render(), filler()})
            }), 
            separatorEmpty()
            })
        );
    });

    auto mainContainer = Container::Tab({
        input,
        err
    }, &somestate);
    
    auto screen_renderer = Renderer(mainContainer, [&] {
        switch (somestate) {
            case 0: return input->Render();
            case 1: return err->Render();
        }
        return text("Invalid View"); 
    });

    screen.Loop(screen_renderer);

    cleanup("");
}

SOCKET connectToServer(const std::string& host, int port){
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    hostent* he = gethostbyname(host.c_str());
    if (!he) {
        return SOCKET_ERROR;
    }
    memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);

    // Create socket
    SOCKET ssock = socket( AF_INET, SOCK_STREAM, 0 );
    if(ssock == INVALID_SOCKET){
        return SOCKET_ERROR;
    }

    // Connect to server 
    if (connect(ssock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR){
        closesocket(ssock);
        return SOCKET_ERROR;
    }    

    return ssock;
}


void winInit(){
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cerr << "Failed to init wsaData";
        std::exit(1);
    }
}

int oldmain(){

    // Initialize WSADATA
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0){
        std::cerr << "Failed to init wsaData";
        return 1;
    }

    // Get server info
    std::string hostname;
    int port;

    std::cout << "Server Hostname: ";
    std::cin >> hostname;
    std::cout << "Server Port: ";
    std::cin >> port;

    // Set up server info
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    hostent* he = gethostbyname(hostname.c_str());
    if (!he) {
        std::cerr << "gethostbyname failed" << std::endl;
        return 1;
    }
    memcpy(&server_addr.sin_addr, he->h_addr, he->h_length);


    // Create socket
    SOCKET ssock = socket( AF_INET, SOCK_STREAM, 0 );
    if(ssock == INVALID_SOCKET){
        cleanup("Failed to initialize socket");
        return 1;
    }

    // Connect to server 
    if (connect(ssock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR){
        closesocket(ssock);
        cleanup("Failed to connect to server");
        return 1;
    }    
    std::cout << "Connected to server!" << std::endl << "******************************" << std::endl;

    // Initialize the chat
    std::thread recvThread(receiveLoop, ssock);
    std::thread sendThread(sendLoop, ssock);

    sendThread.join();
    shutdown(ssock, SD_SEND); 
    recvThread.join();


    closesocket(ssock);
    cleanup("");

    return 0;
}

std::string formatMsg(const std::string& msg, bool other){
    if (other){
        return "[Other Client] " + msg + "\n";
    }
    return "[This Client] " + msg + "\n";
}


void cleanup(std::string message){
    std::cout << message;    
    WSACleanup();
}

void addMsg(const std::string& msg, bool clear){
    static std::list<std::string> msgList;
    int prevsize = static_cast<int>(msgList.size());

    msgList.push_front(msg);
    if(msgList.size() > MAX_QUEUE_SIZE){
        msgList.pop_back();
    }

    for (int i = 0; i < prevsize+1; i++){
    }

    for (auto msg : msgList){
    }

    if (clear) { input.clear(); }

    // Move the cursor to the line after the last message
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD cursorPos = {0, (SHORT)(FIRST_MSG_LINE + static_cast<int>(msgList.size())) };
    SetConsoleCursorPosition(hConsole, cursorPos);
    std::cout << "> " << input;
    cursorPos = { (SHORT)(input.size()+2), (SHORT)(FIRST_MSG_LINE + static_cast<int>(msgList.size())) };
    SetConsoleCursorPosition(hConsole, cursorPos);
    
}

// Loop to recieve data from server
void receiveLoop(SOCKET sock){

    char buffer[SEND_RECV_BUFFER_SIZE];
    std::list<std::string> msgList;

    while (true) {
        
        int bytes = recv(sock, buffer, SEND_RECV_BUFFER_SIZE, 0);

        if (bytes <= 0) break;
        
        // Sleep(3000);
        // buffer[0] = 'p';
        // buffer[1] = 'c';
        // buffer[2] = 'b';
        // int bytes = 3;

        stream_mtx.lock();
        addMsg(formatMsg({buffer, static_cast<size_t>(bytes)}, true), false);
        stream_mtx.unlock();
    }
}

// Loop to send to server to other client.
void sendLoop(SOCKET sock){
    char buffer[SEND_RECV_BUFFER_SIZE];
    std::cout << "> ";
    while (true) {

        char ch;
        
        while (true) {
            ch = _getch();  // Get character without waiting for Enter
    
            if (ch == '\r' || ch == '\n') {  
                break;
            } else if (ch == 8) {  // Backspace
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\b \b";  // Erase character from console
                }
            } else {
                input += ch;
                std::cout << ch;
            }
    
            // (Optional) Real-time processing here
            // e.g., std::cout << "\nCurrent input: " << input << std::endl;
        }

        memcpy(buffer, input.c_str(), input.size());
        stream_mtx.lock();
        int send_size = input.size();
        addMsg(formatMsg({buffer, input.size()}, false), true);
        stream_mtx.unlock();
        
        send(sock, buffer, send_size, 0);
    }
}

