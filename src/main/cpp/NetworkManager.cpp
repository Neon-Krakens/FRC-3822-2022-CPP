#include <wpi/sendable/SendableRegistry.h>
#include <memory>

#include <NetworkManager.h>

void NetworkManager::SetNumber(const char* key, double value)
{
    frc::SmartDashboard::PutNumber(key, value);
}
void NetworkManager::SetBoolean(const char* key, bool value)
{
    frc::SmartDashboard::PutBoolean(key, value);
}
void NetworkManager::SetString(const char* key, std::string_view value)
{
    frc::SmartDashboard::PutString(key, value);
}