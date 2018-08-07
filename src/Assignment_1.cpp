#include "../lib/mbed/mbed.h"
#include "../lib/m3pimaze/m3pimaze.h"
#include "../lib/encoder/encoder.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// PI is not defined in the mbed math files
const double PI = 3.1415926535897931;

// define the m3pi (robot) and Serial (wixel) objects
m3pi robot(p23, p9, p10);
Serial wixel(p28, p27);
Timer timer;

int main() {

    // CHANGE MOTOR SPEED HERE
    const float motorSpeeds[] = {0.8, 1.0}; // motor speeds for robot from left to right motor

    // define constants
    const int calcBreak_ms = 50; // time between calculations in ms
    const int pulsePerRev = 45; // wheel pulese/revolution
    const float axle_cm = 9.6; // axle length in cm
    const float wheel_cm = 3.6; // wheel diameter in cm
    const double pulseDistance = PI * (wheel_cm / pulsePerRev);

    // define variables
    int motorTime_ms; // time since started motors in ms
    int nextCalcTime_ms; // time for next calculation in ms
    int encoder_l; // left rotary encoder value
    int encoder_r; // right rotary encoder value
    float roboH; // robots heading
    float roboVoltage; // robots battery voltage
    double roboX; // robots X co-ord
    double roboY; // robots Y co-ord

    
    // display the 'active' message on robot LCD (program title too long)
    for (int i = 0; i < 3; i++) {

        robot.cls();
        robot.locate(0, 0);
        robot.printf("Roaming!");
        wait(1);
        
    }
   
    // set the Wixel communication speed (baud rate)
    wixel.baud(115200); // use default format = 8,N,1

    // display the program title on the PC
    wixel.printf("Pololu Traveller\n\n");

    // get and display the battery voltage
    roboVoltage = robot.battery();
    wixel.printf("Battery voltage: %.3fV\n", roboVoltage);

    // display the wheel speeds
    wixel.printf("Wheel speeds: Right = %%, Left = %%", wheelSpeeds[0], wheelSpeeds[1]);

    // display the PC column headings
    // ADD CODE HERE

    // transmit the initial robot position to the PC
    // convert the heading to degrees before transmitting
    // ADD CODE HERE
    
    // initialise the encoders
    initEncoder();   
    // pause for 2 seconds before starting
    wait(2);
    
    // turn the motors on, at the required speeds
    robot.left_motor(motorSpeeds[0]);
    robot.right_motor(motorSpeeds[1]);

    // start the timer
    timer.start();
    // set the time for the first calculation
    nextCalcTime_ms = calcBreak_ms;
    
    // the main loop   
    while(1) {

        // wait until it is time for next calculation
        do {

            motorTime_ms = timer.read_ms();

        } while (motorTime_ms < nextCalcTime_ms);
        // set time for the next calculation
        nextCalcTime_ms += calcBreak_ms;

        // get the encoder pulse counts
        encoder_l = leftEncoder();
        encoder_r = rightEncoder();
      
        // display the pulse count values on the LCD
        robot.locate(0, 1);
        ostringstream encoderPulses;
        encoderPulses << "R=" << encoder_r << "L=" << encoder_l;
        robot.printf(encoderPulses.str().c_str());

        // if there were any pulses
        if (encoder_l != 0 || encoder_r != 0) {

            // calculate the change in the robot position
            if (encoder_l == encoder_r) { // if encoder_l equals encoder_r

                // ADD CODE HERE

            } else { // otherwise, encoder_l does not equal encoder_r

                // ADD CODE HERE
                
            }
            // calculate the new robot position
            // ADD CODE HERE

            // if necessary, adjust the heading
            if (roboH < -PI) {

                roboH = roboH + 2 * PI;

            } else if (roboH > PI) {

                roboH = roboH - 2 * PI;

            }
        }
         
        // calculate the current time 
        // ADD CODE HERE
 
        // transmit the new robot position to the PC
        // convert H to degrees before transmitting
        // ADD CODE HERE

    }
}


