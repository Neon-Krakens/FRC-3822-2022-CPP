#pragma once

#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/motorcontrol/PWMSparkMax.h>

#define LEFT_BIAS 1.045
#define SMOOTHING_TIME 6
#define MOTOR_LEFT_CHANNEL 0
#define MOTOR_RIGHT_CHANNEL 1
#define SMOOTHING_ENABLED true
#define DEAD_ZONE 0.05
#define STRAIGHT_MODE_THRESHOLD 0.3

#define STRAIGHT_SPEED 0.5

class SmoothedDifferentialDrive
{ 
public:
    frc::PWMSparkMax LeftMotor{MOTOR_LEFT_CHANNEL};
    frc::PWMSparkMax RightMotor{MOTOR_RIGHT_CHANNEL};
    frc::DifferentialDrive Driver{LeftMotor, RightMotor};
    bool EnableSmoothing;
    void Init();
    void Tick(double leftX, double leftY, double rightX, double rightY, double time, double multiplier, void* robot);
    void DriveForward(double power);
    void StopDriving();
    void Clear();
    static double GetAverage(double* vals, int length);
    static void ClearValues(double* vals, int length);
    SmoothedDifferentialDrive();
    ~SmoothedDifferentialDrive();
    double LastLeftX;
    double LastLeftY;
    double LastRightX;
    double LastRightY;
private:
    double _historyLeftX[SMOOTHING_TIME];
    double _historyLeftY[SMOOTHING_TIME];
    double _historyRightX[SMOOTHING_TIME];
    double _historyRightY[SMOOTHING_TIME];
    int index;
};