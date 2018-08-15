// pololu-cartographer.cpp
// Developed by: Brandon Ash Lovegrove, ID: 17982686
// ENSE504 2018 Assignment 1 program code
// Requires encoder.cpp and encoder.h as a library in the project.

#include "mbed.h"
#include "m3pimaze.h"
#include "encoder.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

// PI is not defined in the mbed math files
const double PI = 3.1415926535897931;

// define the m3pi (robot) and Serial (wixel) objects
m3pi robot(p23, p9, p10);
Serial wixel(p28, p27);

// create timer
Timer timer;

int main() {

    // CHANGE MOTOR SPEED HERE
    const float motorSpeeds[] = {0.7, 0.4}; // motor speeds for robot from left to right motor

    // define constants
    const int calcBreak_ms = 50; // time between calculations in ms
    const int pulsePerRev = 45; // wheel pulese/revolution
    const float axle_cm = 9.6; // axle length in cm
    const float wheel_cm = 3.6; // wheel diameter in cm
    const double pulseDistance = PI * (wheel_cm / pulsePerRev);

    // define variables
    int runTime_ms = 0; // time since started main operation started in milliseconds
    int nextCalcTime_ms; // time for next calculation in ms
    int encoder_l = 0; // left rotary encoder value
    int encoder_r = 0; // right rotary encoder value
    float runTime_s = 0.00; // time since main operation started in seconds
    float roboVoltage; // robots battery voltage
    float roboH_change; // change in heading after movement & recalculation
    float roboH_degrees; // robots heading in degrees
    float roboR; // distance of robot to graph origin (path radius)

    // associative array ('map' or 'dictionary') of robot position for easier handling
    map<string,float> roboPosition;
    roboPosition["X"] = 0.0; // robot X co-ordinate
    roboPosition["Y"] = 0.0; // robot Y co-ordinate
    roboPosition["H"] = 0.0; // robot heading
    
    // display the 'active' message on robot LCD on boot (program title too long)
    for (int i = 0; i < 3; i++) {

        robot.cls();
        wait(.5);
        robot.locate(0, 0);
        robot.printf("Mapping!");
        wait(.5);
    }
   
    // set the Wixel communication speed (baud rate / bitrate)
    wixel.baud(115200);

    // display the program title on the PC
    wixel.printf("Pololu Cartographer\r\n");

    // get and display the battery voltage
    roboVoltage = robot.battery();
    wixel.printf("Battery voltage: %.3f\r\n", roboVoltage);

    // display the wheel speeds
    wixel.printf("Wheel speeds: Right = %.2f, Left = %.2f\r\n\r\n", motorSpeeds[1], motorSpeeds[0]);

    // display the PC column headings
    wixel.printf("Position log: \r\n");
    wixel.printf("          X       Y       H       T    L    R\r\n"); // print formatted column-headers

    // transmit the initial robot position to the PC
    roboH_degrees = roboPosition["H"] * (180 / PI); // convert robo heading to degrees
    wixel.printf("pos,%7.2f,%7.2f,%7.2f,%6.2f,%3d,%3d\r\n", roboPosition["X"], roboPosition["Y"], roboH_degrees, runTime_s, encoder_l, encoder_r);
    
    // initialise the encoders
    InitEncoder();   
    // pause for 2 seconds before starting
    wait(2);
    
    // turn the motors on, at the required speeds
    robot.left_motor(motorSpeeds[0]);
    robot.right_motor(motorSpeeds[1]);
    // motor speeds swapped because left/right mixup in 3pi library

    // start the timer
    timer.start();
    // set the time for the first calculation
    nextCalcTime_ms = calcBreak_ms;
    
    // the main loop   
    while(1) {

        // wait until it is time for next calculation
        do {

            runTime_ms = timer.read_ms();

        } while (runTime_ms < nextCalcTime_ms);
        // set time for the next calculation
        nextCalcTime_ms += calcBreak_ms;

        // get the encoder pulse counts
        encoder_l = LeftEncoder();
        encoder_r = RightEncoder();
      
        // display the pulse count values on the LCD left value = left encoder, right value = right encoder
        robot.locate(0, 1);
        robot.printf("%3d  %3d", encoder_l, encoder_r);

        // if the wheels moved
        if (encoder_l != 0 || encoder_r != 0) {

            // calculate the change in the robot position
            if (encoder_l == encoder_r) { // if encoder_l equals encoder_r

                roboPosition["X"] = (encoder_l * pulseDistance * cos(roboPosition["H"]));
                roboPosition["Y"] = (encoder_l * pulseDistance * sin(roboPosition["H"]));

            } else { // otherwise, encoder_l does not equal encoder_r

                roboR = (0.5 * axle_cm * (encoder_r + encoder_l) / (encoder_r - encoder_l));
                roboH_change = ((encoder_r - encoder_l) * pulseDistance / axle_cm);
                roboPosition["X"] += (roboR * (sin(roboPosition["H"] + roboH_change) - sin(roboPosition["H"])));
                roboPosition["Y"] += (roboR * (cos(roboPosition["H"]) - cos(roboPosition["H"] + roboH_change)));

            }
            
            // calculate the new robot heading
            roboPosition["H"] += roboH_change;

            // if necessary, adjust the heading
            if (roboPosition["H"] < -PI) {

                roboPosition["H"] += 2 * PI;

            } else if (roboPosition["H"] > PI) {

                roboPosition["H"] -= 2 * PI;

            }
        }

        // add one 5ms period to run time for 1 complete calculation
        runTime_s += 0.05; // plain number instead of variable&math for less processor cycles

        // convert robo heading to degrees
        roboH_degrees = roboPosition["H"] * (180 / PI);

        // transmit the new robot position + data to the PC
        wixel.printf("pos,%7.2f,%7.2f,%7.2f,%6.2f,%3d,%3d\r\n", roboPosition["X"], roboPosition["Y"], roboH_degrees, runTime_s, encoder_l, encoder_r);
    }
}


