#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include "QTime"

namespace
{
const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
const int SECONDS_TO_HOURS = 3600;
const int PERCENT_TO_DECMIMAL = 100;
const int HMS_CONVERSION_FACTOR = 60;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
{
}

BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return ampHours_;
}

bool BatteryStateOfChargeService::isCharging() const
{
    if(currentNew_ < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    QTime time;
    double totalHours;
    int hours;
    int remainingHours;
    int minutes;
    int seconds;

    if(isCharging())
        totalHours = qAbs(totalAmpHoursUsed() / currentNew_);
    else
        totalHours = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / currentNew_;

    hours = (int)totalHours;
    minutes = (int)((totalHours - hours) * HMS_CONVERSION_FACTOR);
    seconds = (int)((totalHours * HMS_CONVERSION_FACTOR - hours * HMS_CONVERSION_FACTOR - minutes) * HMS_CONVERSION_FACTOR);

    //uses msec as a place holder for the remaining hours as the hours in QTime could only hold up to 24
    remainingHours = ((int)(totalHours/24)) * 24;
    hours -= remainingHours;

    time.setHMS(hours, minutes, seconds, remainingHours);

    return time;
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    // This is where you can update your variables
    // Hint: There are many different ways that the totalAmpHoursUsed can be updated
    // i.e: Taking a running average of your data values, using most recent data points, etc.
    QTime timeOld = timeNew_;
    double currentOld = currentNew_;

    currentNew_ = batteryData.current;
    timeNew_ = batteryData.time;

    if(timeOld.isNull()) //checks if timeOld has no a value (first itteration)
    {
        ampHours_ += BATTERY_AMP_HOUR_CAPACITY * initialStateOfChargePercent_ / PERCENT_TO_DECMIMAL;
    }
    else
    {
        double avgCurrent = (currentOld + currentNew_) / 2;
        ampHours_ += (avgCurrent * timeOld.secsTo(timeNew_) / SECONDS_TO_HOURS);
    }
}
