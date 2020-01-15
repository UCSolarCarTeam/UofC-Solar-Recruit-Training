#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include <math.h>
namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
    const int HOUR_IN_MILLISECONDS = 3600000;
    const int MINUTE_IN_MILLISECONDS = 60000;
    const int SECOND_IN_MILLISECONDS = 1000;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
: initialStateOfChargePercent_(initialStateOfChargePercent), totalAmphoursUsed_(initialStateOfChargePercent_ / 100 * BATTERY_AMP_HOUR_CAPACITY)
{   
}

BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return totalAmphoursUsed_;
}

bool BatteryStateOfChargeService::isCharging() const
{
    return (current_ < 0);
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    double totalHoursRemaining;
    double millisecondsRemaining;

    if(isCharging())
        totalHoursRemaining = abs(totalAmpHoursUsed() / current_);
    else
        totalHoursRemaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / current_;

    //Checking for total hours to be in between 0 and 24
    while(totalHoursRemaining > 24)
    {
        totalHoursRemaining -= 24;
    }

    //Converting totalHoursRemaining into hours, minutes, seconds, milliseconds
    int hours = floor(totalHoursRemaining);
    millisecondsRemaining = (totalHoursRemaining - hours) * HOUR_IN_MILLISECONDS;
    int minutes = floor(millisecondsRemaining / MINUTE_IN_MILLISECONDS);
    millisecondsRemaining = millisecondsRemaining - (minutes * MINUTE_IN_MILLISECONDS);
    int seconds = floor(millisecondsRemaining / SECOND_IN_MILLISECONDS);
    millisecondsRemaining = millisecondsRemaining - (seconds * SECOND_IN_MILLISECONDS);
    int milliseconds = floor(millisecondsRemaining);

    QTime timeRemaining(hours, minutes, seconds, milliseconds);
    return timeRemaining;
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    // This is where you can update your variables
    // Hint: There are many different ways that the totalAmpHoursUsed can be updated
    // i.e: Taking a running average of your data values, using most recent data points, etc.

    //Calculating inital Amphours used
    double currentPrev = current_;
    current_ = batteryData.current;

    QTime timePrev = time_;
    time_ = batteryData.time;

    double changeInTimeMSecs = timePrev.msecsTo(time_);
    double changeInTimeHours = changeInTimeMSecs / HOUR_IN_MILLISECONDS;

    double currentAmphoursUsed = ((current_ + currentPrev) / 2) * changeInTimeHours;
    totalAmphoursUsed_ = totalAmphoursUsed_ + currentAmphoursUsed;
}
