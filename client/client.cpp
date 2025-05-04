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


constexpr int HOST_BUFFER_SIZE = 256;
constexpr int SEND_RECV_BUFFER_SIZE = 4096;
constexpr int PORT_NUMBER = 54321;
constexpr int MAX_QUEUE_SIZE = 20;
constexpr int FIRST_MSG_LINE = 4;

std::mutex sock_mtx; // Mutex for sockets
std::mutex stream_mtx; // Mutex for sockets
std::string input; // Global input state


void cleanup(std::string message);
void addMsg(const std::string& msg, bool clear);
void receiveLoop(SOCKET sock);
void sendLoop(SOCKET sock);
void deleteLine(int line);
void insertLine(int line, const std::string& text);
std::string formatMsg(const std::string& msg, bool other);



int main(){

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
        deleteLine(FIRST_MSG_LINE);
    }

    for (auto msg : msgList){
        insertLine(FIRST_MSG_LINE, msg);
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

void deleteLine(int line) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    COORD start = { 0, (SHORT)line };
    SMALL_RECT scrollRect = { 0, (SHORT)(line + 1), (SHORT)(csbi.dwSize.X - 1), (SHORT)(csbi.dwSize.Y - 1)};
    CHAR_INFO fill = { ' ', csbi.wAttributes };

    // Scroll up by one line (delete line)
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, nullptr, start, &fill);

    // Clear the last line
    COORD lastLine = { 0, (SHORT)(csbi.dwSize.Y - 1)};
    DWORD charsWritten;
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, lastLine, &charsWritten);
}

void insertLine(int line, const std::string& text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Prepare to scroll content
    SMALL_RECT scrollRect = { 0, (SHORT)line, SHORT(csbi.dwSize.X - 1), (SHORT)(csbi.dwSize.Y - 2) };
    COORD dest = { 0, (SHORT)(line + 1) };
    CHAR_INFO fill = { ' ', csbi.wAttributes };

    // Scroll the content down by one line to make space for the new line
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, nullptr, dest, &fill);

    // Move cursor to the specific line where we want to insert the new text
    COORD cursorPos = { 0, (SHORT)line };
    SetConsoleCursorPosition(hConsole, cursorPos);

    // Write the new text to the inserted line
    DWORD charsWritten;
    WriteConsoleA(hConsole, text.c_str(), text.length(), &charsWritten, nullptr);
}   

