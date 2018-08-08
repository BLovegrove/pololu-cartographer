#include "mbed.h"
#include "m3pimaze.h"
#include "encoder.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// PI is not defined in the mbed math files
const double PI = 3.1415926535897931;

// define the m3pi (robot) and Serial (wixel) objects
m3pi robot(p23, p9, p10);
Serial wixel(p28, p27);
Timer timer;

int main() {

    // CHANGE MOTOR SPEED HERE
    const float motorSpeeds[] = {0.4, 0.7}; // motor speeds for robot from left to right motor

    // define constants
    const int calcBreak_ms = 50; // time between calculations in ms
    const int pulsePerRev = 45; // wheel pulese/revolution
    const float axle_cm = 9.6; // axle length in cm
    const float wheel_cm = 3.6; // wheel diameter in cm
    const double pulseDistance = PI * (wheel_cm / pulsePerRev);

    // define variables
    int runningTime_ms = 0; // time since started motors in ms
    int nextCalcTime_ms; // time for next calculation in ms
    int encoder_l = 0; // left rotary encoder value
    int encoder_r = 0; // right rotary encoder value
    float roboVoltage; // robots battery voltage
    float roboX = 0.0; // robots X co-ord
    float roboX_change; // change in X co-ord after movement & recalculation
    float roboY = 0.0; // robots Y co-ord
    float roboY_change; // change in Yco-ord after movement & recalculation
    float roboH = 0.0; // robots heading
    float roboH_change; // change in heading after movement & recalculation
    float roboH_degrees; // robots heading in degrees
    float roboR; // distance of robot to graph origin (path radius)

    
    // display the 'active' message on robot LCD (program title too long)
    for (int i = 0; i < 3; i++) {

        robot.cls();
        wait(.5);
        robot.locate(0, 0);
        robot.printf("Roaming!");
        wait(.5);
        robot.cls();
        
    }
   
    // set the Wixel communication speed (baud rate / bitrate)
    wixel.baud(115200); // use default format = 8,N,1

    // display the program title on the PC
    wixel.printf("Pololu Traveller\r\n");

    // get and display the battery voltage
    roboVoltage = robot.battery();
    wixel.printf("Battery voltage: %.3f\r\n", roboVoltage);

    // display the wheel speeds
    wixel.printf("Wheel speeds: Right = %d, Left = %d\r\n\r\n", motorSpeeds[1], motorSpeeds[0]);

    // display the PC column headings
    wixel.printf("Position log: \r\n");
    wixel.printf("%8s,-%8s,-%8s,-%8s,-%8s,-%8s,-%8s\r\n", "", "X:", "Y:", "H:", "L:", "R:"); // print formatted column-headers

    // transmit the initial robot position to the PC
    roboH_degrees = roboH * (180 / PI); // convert robo heading to degrees
    wixel.printf("%8s,-%8.2f,-%8.2f,-%8.2f,-%8.2f,-%8d,-%8d\r\n", "POS,", roboX, roboY, roboH_degrees, encoder_l, encoder_r); // transmit robot co-ords / data
    
    // initialise the encoders
    initEncoder();   
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

            runningTime_ms = timer.read_ms();

        } while (runningTime_ms < nextCalcTime_ms);
        // set time for the next calculation
        nextCalcTime_ms += calcBreak_ms;

        // get the encoder pulse counts
        encoder_l = leftEncoder();
        encoder_r = rightEncoder();
      
        // display the pulse count values on the LCD
        ostringstream encoderPulses_l;
        ostringstream encoderPulses_r;
        encoderPulses_l << "L=" << encoder_l;
        encoderPulses_r << "R=" << encoder_r;
        robot.cls();
        robot.locate(0, 0);
        robot.printf(encoderPulses_l.str().c_str());
        robot.locate(0, 1);
        robot.printf(encoderPulses_r.str().c_str());

        // if there were any pulses
        if (encoder_l != 0 || encoder_r != 0) {

            // calculate the change in the robot position
            if (encoder_l == encoder_r) { // if encoder_l equals encoder_r

                roboX = (encoder_l * pulseDistance * cos(roboH));
                roboY = (encoder_l * pulseDistance * sin(roboH));

            } else { // otherwise, encoder_l does not equal encoder_r

                roboR = (0.5 * axle_cm * (encoder_r + encoder_l) / (encoder_r - encoder_l));
                roboH_change = ((encoder_r - encoder_l) * pulseDistance / axle_cm);
                roboX_change = (roboR * (sin(roboH + roboH_change) - sin(roboH)));
                roboY_change = (roboR * (cos(roboH) - cos(roboH + roboH_change)));

            }
            // calculate the new robot position
            roboH += roboH_change;
            roboX += roboX_change;
            roboY += roboY_change;

            // if necessary, adjust the heading
            if (roboH < -PI) {

                roboH = roboH + 2 * PI;

            } else if (roboH > PI) {

                roboH = roboH - 2 * PI;

            }
        }
        // transmit the new robot position + data to the PC
        roboH_degrees = roboH * (180 / PI); // convert robo heading to degrees
        wixel.printf("%8s,-%8.2f,-%8.2f,-%8.2f,-%8.2f,-%8d,-%8d\r\n", "POS,", roboX, roboY, roboH_degrees, encoder_l, encoder_r); // transmit robot co-ords / data

    }
}


