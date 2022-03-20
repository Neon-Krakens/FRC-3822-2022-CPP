
#include <AutoMode.h>
#include <Robot.h>

#define STEP_IDLE 0
#define STEP_MOVE_FORWARD 1
#define STEP_BELT_MOVING 2
#define STEP_MOVE_BACK 3
#define STEP_DONE 4
void AutoMode::Setup()
{
    stepTime = 0;
    step = 0;
}
bool isStepInit = false;
void AutoMode::Tick(double time, void* robot)
{
    Robot* r = (Robot*)robot;
    StepCheck(time);
    time -= stepTime;
    switch (step)
    {
        case STEP_MOVE_FORWARD:
            r->Driver.DriveForward(-DRIVE_FORWARD_POWER);
            break;
        case STEP_BELT_MOVING:
            r->Driver.StopDriving();
            if (isStepInit)
                r->peripheralSystem.StartBelt();
            break;
        case STEP_MOVE_BACK:
            if (isStepInit)
                r->peripheralSystem.StopBelt();
            r->Driver.DriveForward(DRIVE_BACK_POWER);
            break;
        case STEP_DONE:
            r->Driver.StopDriving();
            break;
    }
}
void AutoMode::StepCheck(double time)
{
    double currentTime = time - stepTime;
    switch (step)
    {
        case STEP_IDLE:
            step = STEP_BELT_MOVING;
            goto setStep;
        case STEP_MOVE_FORWARD:
            if (currentTime >= DRIVE_FORWARD_TIME)
            {
                step = STEP_BELT_MOVING;
                goto setStep;
            }
            break;
        case STEP_BELT_MOVING:
            if (currentTime >= BELT_MOVE_TIME)
            {
                step = STEP_MOVE_BACK;
                goto setStep;
            }
            break;
        case STEP_MOVE_BACK:
            if (currentTime >= DRIVE_BACK_TIME)
            {
                step = STEP_DONE;
                goto setStep;
            }
            break;
    }
    isStepInit = false;
    return;
    setStep:
    stepTime = time;
    isStepInit = true;
}