#include "BatteryStateOfChargeService.h"
#include <cmath>

namespace
{
const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
const int HOURS_TO_MILISECONDS = 3600000;
const int MINUTES_TO_MILISECONDS = 60000;
const int SECONDS_TO_MILISECONDS = 1000;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
{
    amphour_ = BATTERY_AMP_HOUR_CAPACITY * initialStateOfChargePercent_ / 100;
    current_ = 0;
}

BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return amphour_;
}

bool BatteryStateOfChargeService::isCharging() const
{
    return (current_ < 0);
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    return QTime(0, minutes, seconds, miliseconds);
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    // This is where you can update your variables
    // Hint: There are many different ways that the totalAmpHoursUsed can be updated
    // i.e: Taking a running average of your data values, using most recent data points, etc.
    double currentOld_ = current_;
    current_ = batteryData.current;

    QTime timeOld_ = time;
    time = batteryData.time;

    double averageAmphour_ = (current_ + currentOld_) / 2;

    //timeOld_ would be null for the first entry of data
    if (!timeOld_.isNull())
    {
        amphour_ += averageAmphour_ * timeOld_.msecsTo(time) / HOURS_TO_MILISECONDS;
    }

    //Calculation of the time until charged/depleted
    if (isCharging())
    {
        totalTimeHoursDouble =  abs(amphour_ / current_);
    }
    else
    {
        totalTimeHoursDouble =  (BATTERY_AMP_HOUR_CAPACITY - amphour_) / current_;
    }

    //Calculating the Time
    miliseconds = totalTimeHoursDouble * HOURS_TO_MILISECONDS;
    hoursInteger = floor(totalTimeHoursDouble);
    miliseconds = miliseconds - (hoursInteger * HOURS_TO_MILISECONDS);
    minutes = floor(miliseconds / MINUTES_TO_MILISECONDS);
    miliseconds = miliseconds - (minutes * MINUTES_TO_MILISECONDS);
    seconds = floor(miliseconds / SECONDS_TO_MILISECONDS);
    miliseconds = miliseconds - (seconds * SECONDS_TO_MILISECONDS);
    miliseconds = floor(miliseconds);
}
