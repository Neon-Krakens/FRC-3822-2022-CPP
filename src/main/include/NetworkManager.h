#pragma once

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableChooser.h>

class NetworkManager
{
public:
    static void SetNumber(const char* key, double value);
    static void SetBoolean(const char* key, bool value);
    static void SetString(const char* key, std::string_view value);
};