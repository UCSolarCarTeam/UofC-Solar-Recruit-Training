#include "BatteryStateOfChargeService.h"
#include <stdio.h>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
    const int HOURS_TO_MSECS = 3600000;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
: initialStateOfChargePercent_(initialStateOfChargePercent)
{
    ampHourTotal = (initialStateOfChargePercent_/100) * BATTERY_AMP_HOUR_CAPACITY;
}

BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return ampHourUsed;
}

bool BatteryStateOfChargeService::isCharging() const
{
    if(currCurrent_>=0)
    {
        return false;
    }
    return true;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    double ampHoursLeft;
    int current = currCurrent_;
    if(isCharging())
    {
        ampHoursLeft = BATTERY_AMP_HOUR_CAPACITY - ampHourTotal;
        current *= -1;
    }
    else
    {
        ampHoursLeft = ampHourTotal;
    }
    double msecLeft = (ampHoursLeft * HOURS_TO_MSECS) / current;
    QTime QTimeLeft = QTime(0,0,0,0).addMSecs(msecLeft);
    return QTimeLeft;
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    if(currTime_.isNull())
    {
        prevTime_ = QTime(0,0,0,0);
    }
    prevCurrent_ = currCurrent_;
    currCurrent_ = batteryData.current;
    prevTime_ = QTime(currTime_.hour(),currTime_.minute(),currTime_.second(),currTime_.msec());
    currTime_ = batteryData.time;

    //ampHourCalculation is put here instead of totalAmpHoursUsed() due to the method being a const
    double diffOfTimeMSec = prevTime_.msecsTo(currTime_);
    double diffOfTime = diffOfTimeMSec/HOURS_TO_MSECS;
    double avgCurrent = (currCurrent_+prevCurrent_)/2;

    double changeInAmpHour = avgCurrent * diffOfTime;
    ampHourTotal += changeInAmpHour;
    ampHourUsed = BATTERY_AMP_HOUR_CAPACITY- ampHourTotal;
}
