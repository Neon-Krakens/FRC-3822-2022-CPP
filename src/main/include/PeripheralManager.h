#pragma once

#include <frc/Encoder.h>
#include <frc/motorcontrol/PWMSparkMax.h>

#define BELT_LEFT_MOTOR_PIN 4
#define BELT_RIGHT_MOTOR_PIN 3

#define FLIP_BELT true

#define PADDLE_MOTOR_PIN 2
#define LIFT_MOTOR_PIN 5
#define PADDLE_ENCODER_PIN_1 0
#define PADDLE_ENCODER_PIN_2 1
#define LIFT_ENCODER_PIN_1 2
#define LIFT_ENCODER_PIN_2 3

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define BELT_MOTOR_SPEED_START 0.55
#define BELT_MOTOR_SPEED_END 0.775
#define BELT_MOTOR_LEFT_OFFSET 0.0
#define BELT_RAMP_TIME 5.

#define LIFT_MOTOR_TICKS_PER_ROTATION 7.
#define LIFT_MOTOR_SPEED -0.7
#define LIFT_INCHES_PER_ROTATION 0.1
#define LIFT_MAX_HEIGHT_INCHES 12.
#define LIFT_MIN_RPM 10
#define LIFT_CALIBRATE_SPEED -0.15

#define PADDLE_MOTOR_TICKS_PER_ROTATION 420.
#define PADDLE_MOTOR_SPEED -0.65
#define PADDLE_CALIBRATE_SPEED 0.25
#define PADDLE_RISE_TARGET_ANGLE 2.2 * PI / 5
#define PADDLE_FALL_TARGET_ANGLE PI / -10
#define PADDLE_START_ANGLE 0
#define PADDLE_IDLE_TARGET_ANGLE PADDLE_FALL_TARGET_ANGLE - PADDLE_START_ANGLE

#define STATE_INIT 1
#define STATE_CALIBRATING 2
#define STATE_IDLE 4
#define STATE_UP 8
#define STATE_DOWN 16

#define STATE_BELT_MOVING_BACK 2
#define STATE_BELT_MOVING 1
#define STATE_BELT_STATIC 0




class PeripheralManager
{
public:
    PeripheralManager();
    void Tick(void* robot);
    void PaddleUp();
    void PaddleDown();
    void StartBelt();
    void StartBeltBack();
    void StopBelt();
    void RaiseLift();
    void LowerLift();
    void StopLift();
    void SetToInitialState();
    frc::PWMSparkMax paddleMotor{PADDLE_MOTOR_PIN};
    frc::PWMSparkMax liftMotor{LIFT_MOTOR_PIN};
    frc::PWMSparkMax beltLeftMotor{BELT_LEFT_MOTOR_PIN};
    frc::PWMSparkMax beltRightMotor{BELT_RIGHT_MOTOR_PIN};
private:
    void TickPaddle();
    void SetPaddle(double speed);
    frc::Encoder paddleEncoder{PADDLE_ENCODER_PIN_1, PADDLE_ENCODER_PIN_2, false, frc::Encoder::EncodingType::k4X};
    frc::Encoder liftEncoder{LIFT_ENCODER_PIN_1, LIFT_ENCODER_PIN_2, false, frc::Encoder::EncodingType::k4X};
    double time;
    bool beltSpeedState = false;
    bool hasRegState = false;
    int paddleState = STATE_INIT;
    int liftState = STATE_INIT;
    int beltState = STATE_BELT_STATIC;
};