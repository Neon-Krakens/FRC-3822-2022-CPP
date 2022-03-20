#pragma once
#include <string>
//#define ENABLE_RUNWAY
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/Joystick.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <SmoothedDifferentialDrive.h>
#include <NetworkManager.h>
#include <Controls.h>
#include <PeripheralManager.h>
#include <AutoMode.h>
#ifdef ENABLE_RUNWAY
#include <Runway.h>
#endif

#include <frc/Encoder.h>

#define JOYSTICK_LEFT_USB 0
#define JOYSTICK_RIGHT_USB 1


class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void DisabledInit() override;
  void DisabledPeriodic() override;
  void TestInit() override;
  void TestPeriodic() override;
  void PlayRunwayAnimation();
  bool IsAuto = false;
  frc::Joystick m_joystickLeft{JOYSTICK_LEFT_USB};
  frc::Joystick m_joystickRight{JOYSTICK_RIGHT_USB};
  PeripheralManager peripheralSystem;
  SmoothedDifferentialDrive Driver{};
  AutoMode autoMode{};
  static Robot* Instance;
#ifdef ENABLE_RUNWAY
  Runway runway{};
#endif
  double period;
  double time;
  double autoTime;
  double periodicTime;
 private:
  bool isBackwards;
};
