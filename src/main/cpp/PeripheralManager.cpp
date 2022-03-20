#include <Robot.h>
#include <PeripheralManager.h>
    
PeripheralManager::PeripheralManager()
{
    paddleEncoder.SetDistancePerPulse((2. * PI) / PADDLE_MOTOR_TICKS_PER_ROTATION);
    paddleEncoder.SetMinRate(PI / 4);
    liftEncoder.SetDistancePerPulse(LIFT_INCHES_PER_ROTATION / LIFT_MOTOR_TICKS_PER_ROTATION);
    liftEncoder.SetMinRate(LIFT_INCHES_PER_ROTATION * LIFT_MIN_RPM);
    beltRightMotor.SetInverted(!FLIP_BELT);
    beltLeftMotor.SetInverted(FLIP_BELT);
    paddleState = STATE_INIT | STATE_IDLE;
    liftState = STATE_INIT | STATE_IDLE;
}
double paddleTarget;
double paddlePosition;
int paddleTimeout = 0;
double paddleLastSpeed = 0;
void PeripheralManager::SetPaddle(double speed)
{
    paddleMotor.Set(speed);
    paddleLastSpeed = speed;
}
void PeripheralManager::SetToInitialState()
{
    paddleState = STATE_CALIBRATING;
    paddleEncoder.Reset();
    SetPaddle(PADDLE_CALIBRATE_SPEED);
    paddleTimeout = 0;
    paddleTarget = 0;
    paddlePosition = 0;
    
    fmt::print("Calibration started\n");
}
bool lastBeltControlState = false;
bool lastBeltBackControlState = false;
void PeripheralManager::Tick(void* robot)
{
    TickPaddle();
    
    if (beltState == STATE_BELT_MOVING)
    {
        bool old = beltSpeedState;
        beltSpeedState = Controls::SpeedBelt.GetValue(robot) || Robot::Instance->IsAuto;
        if (old != beltSpeedState) hasRegState = false;
        if (!hasRegState)
        {
            double speed = beltSpeedState ? BELT_MOTOR_SPEED_END : BELT_MOTOR_SPEED_START;
            beltLeftMotor.Set(speed + BELT_MOTOR_LEFT_OFFSET);
            beltRightMotor.Set(speed);
        }
    }
    if (Controls::LowerPaddle.GetValue(robot))
        PaddleDown();
    else if (Controls::RaisePaddle.GetValue(robot))
        PaddleUp();
    if (Controls::LowerLift.GetValue(robot))
        LowerLift();
    else if (Controls::RaiseLift.GetValue(robot))
        RaiseLift();
    else
        StopLift();
    if (lastBeltControlState != Controls::ToggleBelt.GetValue(robot))
    {
        if (lastBeltControlState) StopBelt();
        else StartBelt();
        lastBeltControlState = !lastBeltControlState;
        lastBeltBackControlState = false;
    }
    else if (lastBeltBackControlState != Controls::BackBelt.GetValue(robot))
    {
        if (lastBeltBackControlState) StopBelt();
        else StartBeltBack();
        lastBeltBackControlState = !lastBeltBackControlState;
        lastBeltControlState = false;
    }
}

void PeripheralManager::TickPaddle()
{
    if (paddleTimeout > -1) paddleTimeout++;
    if (paddleTimeout > 5) paddleTimeout = -1;
    if ((paddleState & STATE_IDLE) == STATE_IDLE) return;
    if ((paddleState & STATE_CALIBRATING) == STATE_CALIBRATING)
    {
        if ((paddleEncoder.GetStopped() && paddleTimeout == -1))
        {
            paddlePosition = PADDLE_IDLE_TARGET_ANGLE;
            paddleTarget = PADDLE_IDLE_TARGET_ANGLE + PADDLE_START_ANGLE;
            SetPaddle(PADDLE_MOTOR_SPEED);
            paddleState = 0;
            paddleTimeout = 2;
            paddleEncoder.Reset();
            fmt::print("Done calibrating paddle.\n");
        }
        
        return;
    }
    paddlePosition = paddleEncoder.GetDistance() + PADDLE_IDLE_TARGET_ANGLE;
    if ((paddleLastSpeed > 0 && paddlePosition <= paddleTarget) || 
        (paddleLastSpeed < 0 && paddlePosition >= paddleTarget) || 
        (paddleEncoder.GetStopped() && paddleTimeout == -1))
    {
        paddleTarget = 0;
        paddlePosition = 0;
        paddleState &= ~STATE_CALIBRATING;
        SetPaddle(0);
        paddleState |= STATE_IDLE;
    }
}

void PeripheralManager::PaddleUp()
{
    if ((paddleState & STATE_UP) == STATE_UP) return;
    SetPaddle(PADDLE_MOTOR_SPEED);
    paddleTimeout = 0;
    paddleTarget = PADDLE_RISE_TARGET_ANGLE;
    paddleState = STATE_UP;
}

void PeripheralManager::PaddleDown()
{
    if ((paddleState & STATE_CALIBRATING) == STATE_CALIBRATING) return;
    paddleState = STATE_CALIBRATING;
    paddleEncoder.Reset();
    SetPaddle(PADDLE_CALIBRATE_SPEED);
    paddleTimeout = 0;
    paddleTarget = 0;
    paddlePosition = 0;
    return;
    if ((paddleState & STATE_DOWN) == STATE_DOWN) return;
    SetPaddle(-PADDLE_MOTOR_SPEED);
    paddleTimeout = 0;
    paddleTarget = PADDLE_FALL_TARGET_ANGLE;
    paddleState = STATE_DOWN;
}
void PeripheralManager::StopBelt()
{
    if (beltState == STATE_BELT_MOVING_BACK || beltState == STATE_BELT_MOVING)
    {
        beltSpeedState = false;
        frc::SmartDashboard::PutBoolean("Belt Moving", false);
        beltLeftMotor.Set(0);
        beltRightMotor.Set(0);
        beltState = STATE_BELT_STATIC;
    }
}
void PeripheralManager::StartBelt()
{
    if (beltState == STATE_BELT_STATIC || beltState == STATE_BELT_MOVING_BACK)
    {
        beltSpeedState = false;
        frc::SmartDashboard::PutBoolean("Belt Moving", true);
        double speed = BELT_MOTOR_SPEED_START + BELT_MOTOR_LEFT_OFFSET;
        beltLeftMotor.Set(speed);
        beltRightMotor.Set(speed);
        Robot::Instance->PlayRunwayAnimation();
        beltState = STATE_BELT_MOVING;
    }
}
void PeripheralManager::StartBeltBack()
{
    if (beltState == STATE_BELT_STATIC || beltState == STATE_BELT_MOVING)
    {
        beltSpeedState = false;
        frc::SmartDashboard::PutBoolean("Belt Moving", true);
        double speed = -(BELT_MOTOR_SPEED_START + BELT_MOTOR_LEFT_OFFSET);
        beltLeftMotor.Set(speed);
        beltRightMotor.Set(speed);
        beltState = STATE_BELT_MOVING_BACK;
    }
}
double liftSpeed = 0.;
void PeripheralManager::RaiseLift()
{
    if (liftSpeed == LIFT_MOTOR_SPEED) return;
    liftSpeed = LIFT_MOTOR_SPEED;
    liftMotor.Set(LIFT_MOTOR_SPEED);
    liftState = STATE_BELT_MOVING;
}
void PeripheralManager::LowerLift()
{
    if (liftSpeed == -LIFT_MOTOR_SPEED) return;
    liftSpeed = -LIFT_MOTOR_SPEED;
    liftMotor.Set(-LIFT_MOTOR_SPEED);
    liftState = STATE_BELT_MOVING;
}
void PeripheralManager::StopLift()
{
    if (liftSpeed == 0) return;
    liftSpeed = 0;
    liftMotor.Set(0);
    liftState = STATE_BELT_STATIC;
}