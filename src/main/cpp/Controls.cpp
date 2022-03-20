#include <Controls.h>

#include <Robot.h>


const Control Controls::LowerPaddle     = Control(2,  JOYSTICK_LEFT,  TYPE_ON_RELEASE);
const Control Controls::RaisePaddle     = Control(3,  JOYSTICK_LEFT,  TYPE_ON_RELEASE);
const Control Controls::ToggleBelt      = Control(1,  JOYSTICK_LEFT,  TYPE_IS_DOWN);
const Control Controls::BackBelt        = Control(7,  JOYSTICK_LEFT,  TYPE_IS_DOWN);
const Control Controls::FlipDirections  = Control(10, JOYSTICK_LEFT,  TYPE_ON_RELEASE);
const Control Controls::DisableStraight = Control(2,  JOYSTICK_RIGHT, TYPE_IS_DOWN);
const Control Controls::SpeedBelt       = Control(1,  JOYSTICK_RIGHT, TYPE_IS_DOWN);
const Control Controls::RaiseLift       = Control(3,  JOYSTICK_RIGHT, TYPE_IS_DOWN);
const Control Controls::LowerLift       = Control(2,  JOYSTICK_RIGHT, TYPE_IS_DOWN);
const Control Controls::DriveStraight   = Control(10, JOYSTICK_RIGHT, TYPE_IS_DOWN);


Control::Control(int index, int controlType, int type)
{
    this->ControlType = controlType;
    this->Index = index;
    this->Type = type;
}

bool Control::GetValue(void* robot) const
{
    Robot* r2 = (Robot*)robot;
    frc::Joystick* joystick = nullptr;
    switch (ControlType)
    {
        case (JOYSTICK_LEFT):
            joystick = &(r2->m_joystickLeft);
            break;
        case (JOYSTICK_RIGHT):
            joystick = &(r2->m_joystickRight);
            break;
        default:
            return false;
    }
    switch (Type)
    {
        case (TYPE_IS_DOWN):
            return joystick->GetRawButton(Index);
        case (TYPE_ON_DOWN):
            return joystick->GetRawButtonPressed(Index);
        case (TYPE_ON_RELEASE):
            return joystick->GetRawButtonReleased(Index);
        default:
            return false;
    }
}