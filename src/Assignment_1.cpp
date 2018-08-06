#include "mbed.h"
#include "m3pimaze.h"
#include "encoder.h"

// PI is not defined in the mbed math files
const double PI = 3.1415926535897931;

// define the m3pi (robot) and Serial (wixel) objects
m3pi robot(p23, p9, p10);
Serial wixel(p28, p27);
Timer timer;

int main()
{
    // define constants
    const int calcTime_ms = 50; // time between calculations in ms
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
    double roboX; // robots X co-ord
    double roboY; // robots Y co-ord

    
    // display the program title on the LCD
    // ADD CODE HERE
   
    // set the Wixel communication speed (baud rate)
    wixel.baud(115200); // use default format = 8,N,1

    // display the program title on the PC
    // ADD CODE HERE

    // get and display the battery voltage
    // ADD CODE HERE

    // display the wheel speeds
    // ADD CODE HERE

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
    // ADD CODE HERE

    // start the timer
    timer.start();
    // set the time for the first calculation
    nextCalcTime_ms = calcTime_ms;
    
    // the main loop   
    while(1) {
        // wait until it is time for next calculation
        do {
            motorTime_ms = timer.read_ms();
        } while (motorTime_ms < nextCalcTime_ms);
        // set time for the next calculation
        nextCalcTime_ms += calcTime_ms;

        // get the encoder pulse counts
        encoder_l = leftEncoder();
        encoder_r = rightEncoder();
      
        // display the pulse count values on the LCD
        // ADD CODE HERE

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


