#include <shared/socket_definitions.h>

#include <iostream>
#include "ClientApp.h"

int main() {

    netstart();

    ClientApp clientApp;
    clientApp.Run();

    netshutdown();

}