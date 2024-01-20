/*
 * BlueSwitchTask.c
 *
 *  Created on: Nov. 23, 2022
 *      Author: marce
 */

#include "BlueSwitchTask.h"

static const uint32_t BLUE_LED_EXID = 0xAAAAAAA;

void blueSwitchTask(void const* arg)
{
    uint8_t data[1] = {0x89};

    for (;;)
    {
    	if (HAL_GPIO_ReadPin(BLUE_SWITCH_GPIO_Port, BLUE_SWITCH_Pin)) {
    		data[0] = 0x89;
    	} else {
    		data[0] = 0x0;
    	}

    	if (osMutexWait(SPIMutexHandle, 0) == osOK) {
			CANMsg msg = {.extendedID = BLUE_LED_EXID, .DLC = 1, .data = data};
    		sendExtendedCANMessage(&msg);
			osMutexRelease(SPIMutexHandle);
    	}
    }
}