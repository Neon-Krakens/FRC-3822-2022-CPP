#include <Runway.h>

#include <Robot.h>
#include <frc/AddressableLED.h>

double offset = 0;
double startTime = 0;


Runway::Runway()
{
    ledsLeft.SetLength(LED_STRIP_LENGTH_LIGHTS);
    ledsRight.SetLength(LED_STRIP_LENGTH_LIGHTS);
}
void Runway::StartAnimation()
{
    isActive = true;
    startTime = 0;
    ledsLeft.SetData(ledBuffer);
    ledsRight.SetData(ledBuffer);
}
void Runway::Tick(void* robot)
{
    if (!isActive) return;
    Robot* r = (Robot*)robot;
    if (startTime == 0)
    {
        startTime = r->periodicTime;
        ledsLeft.Start();
        ledsRight.Start();
    }
    else if (r->periodicTime - startTime < ACTIVE_TIME)
    {
        for (int i = 0; i < LED_STRIP_LENGTH_LIGHTS; ++i)
            ledBuffer[i].SetRGB(OFF);
        isActive = false;
        ledsLeft.Stop();
        ledsRight.Stop();
        return;
    }
    offset += LED_PROGRESS_SPEED_LIGHTS_PER_SEC * 0.02;
    if (offset > LED_STRIP_LENGTH_LIGHTS)
        offset = 0;
    int offset2 = (int)offset;
    for (int i = 0; i < LED_STRIP_LENGTH_LIGHTS; ++i)
    {
        if (i > offset2 - RUNWAY_SPACING * 2 && i < offset2 + RUNWAY_SPACING * 2 && i % RUNWAY_SPACING == 0)
            ledBuffer[i].SetRGB(ON);
        else
            ledBuffer[i].SetRGB(OFF);
    }
    ledsLeft.SetData(ledBuffer);
    ledsRight.SetData(ledBuffer);
}