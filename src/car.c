#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <wiringPi.h>
#include "car.h"

void car_init()
{
    //shezhi
    wiringPiSetup();
    pinMode(1,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);

    softPwmCreate(1,1,500);
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);
}
void car_2_left()
{
    softPwmWrite(4,250); //左轮
    softPwmWrite(1,0);
    softPwmWrite(6,0); //右轮前进
    softPwmWrite(5,250);
}
void car_2_up()
{
    softPwmWrite(4,0); //左轮前进
    softPwmWrite(1,250);
    softPwmWrite(6,0); //右轮前进
    softPwmWrite(5,250);
}
void car_2_right()
{
    softPwmWrite(4,0); //左轮前进
    softPwmWrite(1,250);
    softPwmWrite(6,250); //右轮
    softPwmWrite(5,0);
}
void car_2_down()
{
    softPwmWrite(1,250); //左轮back
    softPwmWrite(4,0);
    softPwmWrite(5,250); //右轮back
    softPwmWrite(6,0);
}
void car_2_stop()
{
    softPwmWrite(1,0); //左轮
    softPwmWrite(4,0);
    softPwmWrite(5,0); //stop
    softPwmWrite(6,0);
}
