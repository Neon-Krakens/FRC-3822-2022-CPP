#include "SmoothedDifferentialDrive.h"

#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <Controls.h>

SmoothedDifferentialDrive::SmoothedDifferentialDrive()
{
    this->RightMotor.SetInverted(true);
}
SmoothedDifferentialDrive::~SmoothedDifferentialDrive()
{
    
}
void SmoothedDifferentialDrive::Clear()
{
    ClearValues(_historyLeftX, SMOOTHING_TIME);
    ClearValues(_historyLeftY, SMOOTHING_TIME);
    ClearValues(_historyRightX, SMOOTHING_TIME);
    ClearValues(_historyRightY, SMOOTHING_TIME);
    Driver.TankDrive(0, 0);
}
void SmoothedDifferentialDrive::Tick(double leftX, double leftY, double rightX, double rightY, double time, double multiplier, void* robot)
{
    if (Controls::DriveStraight.GetValue(robot))
    {
        Driver.TankDrive(STRAIGHT_SPEED * LEFT_BIAS, STRAIGHT_SPEED / LEFT_BIAS);
        return;
    }
    else if (EnableSmoothing)
    {
        double diff = leftY - rightY;
        if (std::abs(diff) <= multiplier * STRAIGHT_MODE_THRESHOLD && !Controls::DisableStraight.GetValue(robot))
        {
            leftY = leftY + diff / 2;
            rightY = leftY - diff / 2;
        }
        _historyLeftX[index] = leftX * multiplier;
        _historyLeftY[index] = leftY * multiplier;
        _historyRightX[index] = rightX * multiplier;
        _historyRightY[index] = rightY * multiplier;
        LastLeftX = GetAverage(_historyLeftX, SMOOTHING_TIME);
        LastLeftY = GetAverage(_historyLeftY, SMOOTHING_TIME);
        LastRightX = GetAverage(_historyRightX, SMOOTHING_TIME);
        LastRightY = GetAverage(_historyRightY, SMOOTHING_TIME);
    }
    else
    {
        LastLeftY = leftY * multiplier;
        LastLeftX = leftX * multiplier;
        LastRightY = rightY * multiplier;
        LastRightX = rightX * multiplier;
    }
    if (LastLeftX > -DEAD_ZONE && LastLeftX < DEAD_ZONE)
        LastLeftX = 0;
    if (LastLeftY > -DEAD_ZONE && LastLeftY < DEAD_ZONE)
        LastLeftY = 0;
    if (LastRightX > -DEAD_ZONE && LastRightX < DEAD_ZONE)
        LastRightX = 0;
    if (LastRightY > -DEAD_ZONE && LastRightY < DEAD_ZONE)
        LastRightY = 0;
    Driver.TankDrive(LastLeftY * LEFT_BIAS, LastRightY / LEFT_BIAS, false);
    index = (index + 1) % SMOOTHING_TIME;
}
void SmoothedDifferentialDrive::DriveForward(double power)
{
    Driver.TankDrive(power * LEFT_BIAS, power / LEFT_BIAS);
}
void SmoothedDifferentialDrive::StopDriving()
{
    Driver.TankDrive(0, 0, false);
}
double SmoothedDifferentialDrive::GetAverage(double* values, int length)
{
    double total = 0;
    for (int i = 0; i < length; i++)
    {
        total += *(values + i);
    }
    return total / length;
}
void SmoothedDifferentialDrive::ClearValues(double* values, int length)
{
    for (; length >= 0; length--)
    {
        *(values + length) = 0;
    }
}