#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include "socket_server.h"
#include "car.h"

void message_callback(char *msg)
{
    if(strncmp(msg,"left",strlen("left")) == 0)
    {
        car_2_left();
        softPwmWrite(4,250); //左轮
        softPwmWrite(1,0);
        softPwmWrite(6,0); //右轮前进
        softPwmWrite(5,250);
        printf("left\n");
    }
    else if(strncmp(msg,"right",strlen("right")) == 0)
    {
        car_2_right();
        softPwmWrite(4,0); //左轮前进
        softPwmWrite(1,250);
        softPwmWrite(6,250); //右轮
        softPwmWrite(5,0);
        printf("right\n");
    }
    else if(strncmp(msg,"up",strlen("up")) == 0)
    {
        car_2_up();
        softPwmWrite(4,0); //左轮前进
        softPwmWrite(1,250);
        softPwmWrite(6,0); //右轮前进
        softPwmWrite(5,250);
        printf("up\n");
    }
    else if(strncmp(msg,"down",strlen("down")) == 0)
    {
        car_2_down();
        softPwmWrite(1,250); //左轮back
        softPwmWrite(4,0);
        softPwmWrite(5,250); //右轮back
        softPwmWrite(6,0);
        printf("down\n");
    }
    else if(strncmp(msg,"stop",strlen("stop")) == 0)
    {
        car_2_stop();
        softPwmWrite(1,0); //左轮
        softPwmWrite(4,0);
        softPwmWrite(5,0); //stop
        softPwmWrite(6,0);
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
    wiringPiSetup();
    pinMode(1,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    softPwmCreate(1,1,500);
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);
    socket_server_p server = socket_server_create(6000,TCP);

    socket_server_set_msgcallback(server,message_callback);
    socket_server_start(server);
    getchar();
    return 0;
}
