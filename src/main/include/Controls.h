#pragma once

#include <Robot.h>

#define JOYSTICK_LEFT 1
#define JOYSTICK_RIGHT 2

#define TYPE_IS_DOWN 1
#define TYPE_ON_DOWN 2
#define TYPE_ON_RELEASE 3

struct Control
{
public:
    int ControlType;
    int Index;
    int Type;
    Control(int index, int controlType, int type);
    bool GetValue(void* robot) const;
};

class Controls
{
public:
    static const Control ToggleBelt;
    static const Control BackBelt;
    static const Control RaiseLift;
    static const Control LowerLift;
    static const Control RaisePaddle;
    static const Control LowerPaddle;
    static const Control FlipDirections;
    static const Control DisableStraight;
    static const Control SpeedBelt;
    static const Control DriveStraight;
};
