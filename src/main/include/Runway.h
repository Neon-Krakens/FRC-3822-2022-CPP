#pragma once
#include <Robot.h>

#include <frc/AddressableLED.h>
#include <frc/SerialPort.h>

#define RUNWAY_SPACING 4
#define ON 255, 255, 77
#define OFF 0, 0, 0
#define LED_STRIP_PIN_LEFT 6
#define LED_STRIP_PIN_RIGHT 7
#define LED_STRIP_LENGTH_LIGHTS 50
#define LED_PROGRESS_SPEED_LIGHTS_PER_SEC 10
#define ACTIVE_TIME 5

class Runway
{
public:
    void StartAnimation();
    void Tick(void* robot);
    Runway();
private:
    bool isActive = false;
    frc::AddressableLED ledsLeft{LED_STRIP_PIN_LEFT};
    frc::AddressableLED ledsRight{LED_STRIP_PIN_RIGHT};
    frc::AddressableLED::LEDData ledBuffer[LED_STRIP_LENGTH_LIGHTS];
};