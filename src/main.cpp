#include "mbed.h"
#include "m3pimaze.h"
#include <string>
#include <sstream>
using namespace std;

m3pi robot(p23, p9, p10);
DigitalOut blueLed(LED1);
int boogieCount = 0;
int counter = 0;
int slowDown(float seconds, float startSpeed, string direction);
int boogieStatus = 0;
//direction: compass directions N,S,E,W.

int main(){
    
    robot.cls();
    robot.locate(0, 0);
    robot.printf("ready to");
    robot.locate(0, 1);
    robot.printf("boogie!");
    wait(0.6);
    while(boogieStatus == 0) {
        robot.forward(.5);
        slowDown(1.0, 0.5, "N");
        wait(.5);
        robot.cls();
        robot.locate(0, 0);
        if (boogieStatus == 0) {
            robot.printf("0");
        } else if (boogieStatus == 1) {
            robot.printf("1");
        } else if (boogieStatus == 2) {
            robot.printf("2");
        }
        wait(1);
        /*
        robot.right(.4);
        wait(.5);
        robot.stop();
        robot.left(.5);
        wait(.37);
        robot.stop();
        robot.forward(.4);
        wait(.5);
        robot.stop();
        boogieCount += 1;
        */
    }
}

int slowDown(float seconds, float startSpeed, string direction) {
    
    float speedDecriment = (0.05);
    float speedCurrent_L = robot.getSpeed_L();
    float speedCurrent_R = robot.getSpeed_R();
    int status = 1;
    
    //#slowdir
    //directions: compass directions (E.G: N,E,S,W)
    
    while (speedCurrent_L > 0.0) {
        if (direction == "N") {
            speedCurrent_L -= speedDecriment;
            robot.forward(speedCurrent_L);
            //wait(seconds / speedDecriment);
            //robot.stop();
            wait(.1);
        } else {
            robot.stop();
            robot.cls();
            robot.locate(0, 0);
            robot.printf("error! :");
            robot.locate(0, 1);
            robot.printf("#slowdir");
            status = 2;
        }
    }
    
    if (status == 1) {status = 0;}
    
    return status;
}