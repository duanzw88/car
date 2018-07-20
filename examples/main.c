#include <stdio.h>
#include <string.h>
#include "socket_server.h"
#include "car.h"

void message_callback(char *msg)
{
    if(strncmp(msg,"left",strlen("left")) == 0)
    {
        car_2_left();
        printf("left\n");
    }
    else if(strncmp(msg,"right",strlen("right")) == 0)
    {
        car_2_right();
        printf("right\n");
    }
    else if(strncmp(msg,"up",strlen("up")) == 0)
    {
        car_2_up();
        printf("up\n");
    }
    else if(strncmp(msg,"down",strlen("down")) == 0)
    {
        car_2_down();
        printf("down\n");
    }
    else if(strncmp(msg,"stop",strlen("stop")) == 0)
    {
        car_2_stop();
        printf("stop\n");
    }
    else
    {
        printf("msg = %s\n",msg);
    }

}
int main()
{
    car_init();
    socket_server_p server = socket_server_create(6000,TCP);

    socket_server_set_msgcallback(server,message_callback);
    socket_server_start(server);
    getchar();
    return 0;
}
