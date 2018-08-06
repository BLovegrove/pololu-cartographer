// functions for the AUT encoder
#include "mbed.h"
#include "encoder.h"

// pulse counter variables for the interrupt functions
volatile int leftPulseCount;
volatile int rightPulseCount;

// interrupt input pins
InterruptIn leftRisingEdge(p11);
InterruptIn rightRisingEdge(p12);

// interrupt functions
void LeftPulseRising(void)
{
    leftPulseCount++;
}
void RightPulseRising(void)
{
    rightPulseCount++;
}

// initialise the encoders
void InitEncoder()
{
    leftPulseCount = 0;
    rightPulseCount = 0;
    // attach the interrupt functions
    leftRisingEdge.rise(LeftPulseRising);
    rightRisingEdge.rise(RightPulseRising);
    // clear the interrupt pulse counts
    leftRisingEdge.disable_irq();
    leftPulseCount = 0;
    leftRisingEdge.enable_irq();
    rightRisingEdge.disable_irq();
    rightPulseCount = 0;
    rightRisingEdge.enable_irq();
}

// get and reset the encoder pulse counts
int LeftEncoder(void)
{
    int count;
    leftRisingEdge.disable_irq();
    count = leftPulseCount;
    leftPulseCount = 0;
    leftRisingEdge.enable_irq();
    return count;
}
int RightEncoder(void)
{
    int count;
    rightRisingEdge.disable_irq();
    count = rightPulseCount;
    rightPulseCount = 0;
    rightRisingEdge.enable_irq();
    return count;
}
