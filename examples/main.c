#include <stdio.h>
#include "socket_server.h"
int main()
{
    socket_server_p server = socket_server_create(6000,TCP);
    socket_server_start(server);

    getchar();
    return 0;
}
