#pragma once

#define DRIVE_FORWARD_TIME 0
#define DRIVE_FORWARD_POWER 0.5
#define DRIVE_BACK_TIME 1.9
#define DRIVE_BACK_POWER 0.7
#define BELT_MOVE_TIME 2.25

class AutoMode
{
public:
    void Tick(double time, void* robot);
    void Setup();
private:
    int step = 0;
    double stepTime = 0.;
    void StepCheck(double time);
};