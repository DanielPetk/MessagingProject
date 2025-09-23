#include <shared/socket_definitions.h>
#include <iostream>
#include "ServerApp.h"

int main() {

    netstart();

    ServerApp server;
    server.Run();

    netshutdown();
}