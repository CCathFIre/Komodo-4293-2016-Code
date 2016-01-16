#ifndef _ELEVATOR_H
#define _ELEVATOR_H
#include "Definitions.h"
#include "WPILib.h"

class Elevator
{
public:
Elevator(unsigned int jaguar, Encoder &upEncoder, unsigned int button);
bool UpTick();
bool DownTick();
void GoUp();
void GoDown();
float Stop(float Speed);
bool EStopButton();
int loopCount;
float target;

private:
Elevator();
Jaguar motor;
Encoder * upEncoder;
AnalogInput stopButton;
DigitalInput eStop;

};
#endif
