// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <Robot.h>
#include <fmt/core.h>

#include <frc/smartdashboard/SmartDashboard.h>
#include <NetworkManager.h>

#include <PeripheralManager.h>

Robot* Robot::Instance;

void Robot::RobotInit()
{
  Robot::Instance = this;
  peripheralSystem.paddleMotor.SetInverted(true);
  time = 0;
}
void Robot::RobotPeriodic()
{
  period = (double)GetPeriod();
  time += period;
}
void Robot::AutonomousInit()
{
  IsAuto = true;
  autoTime = 0;
  peripheralSystem.SetToInitialState();
  autoMode.Setup();
}

void Robot::AutonomousPeriodic()
{
  autoTime += period;
  peripheralSystem.Tick(this);
  autoMode.Tick(autoTime, this);
}

void Robot::TeleopInit()
{
  IsAuto = false;
  periodicTime = 0;
  peripheralSystem.SetToInitialState();
}
void Robot::PlayRunwayAnimation()
{
#ifdef ENABLE_RUNWAY
  runway.StartAnimation();
#endif
}
void Robot::TeleopPeriodic()
{
  periodicTime += period;
  if (Controls::FlipDirections.GetValue(this))
  {
    isBackwards = !isBackwards;
  }
  double leftX = m_joystickLeft.GetRawAxis(0);
  double leftY = m_joystickLeft.GetRawAxis(1);
  double leftZ = m_joystickLeft.GetRawAxis(2);
  double rightX = m_joystickRight.GetRawAxis(0);
  double rightY = m_joystickRight.GetRawAxis(1);
  if (isBackwards)
  {
    leftY = -leftY;
    rightY = -rightY;
  }
  Driver.Tick(leftX, leftY, rightX, rightY, time, leftZ, this);
  NetworkManager::SetNumber("Left Tank Forward", Driver.LastLeftY);
  NetworkManager::SetNumber("Right Tank Forward", Driver.LastRightY);
  peripheralSystem.Tick(this);
  #ifdef ENABLE_RUNWAY
  runway.Tick(this);
  #endif
}
void Robot::DisabledInit() 
{
  IsAuto = false;
  Driver.Clear();
}

void Robot::DisabledPeriodic()
{

}

void Robot::TestInit()
{
  IsAuto = false;
}

void Robot::TestPeriodic()
{
  // TODO: make a test function to drive forward so we can tune the left bias
}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif