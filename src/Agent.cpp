/*
 * Agent.cpp
 * Abstract agent interface to an active agent object that runs as
 * FreeRTOS task
 *  Created on: 15 Aug 2022
 *      Author: jondurrant
 */

#include "Agent.h"
#include <string.h>
#include "hardware/structs/sio.h"

/***
 * Constructor
 */
Agent::Agent() {
	// NOP
}

/***
 * Destructor
 */
Agent::~Agent() {
	stop();
}

/***
 * Stop task
 * @return
 */
void Agent::stop() {
	if (xHandle != NULL) {
		vTaskDelete(  xHandle );
		xHandle = NULL;
	}
}

/***
* Get high water for stack
* @return close to zero means overflow risk
*/
unsigned int Agent::getStackHighWater() {
	return (xHandle != NULL) ? uxTaskGetStackHighWaterMark(xHandle) : 0;
}

/***
* Get the FreeRTOS task being used
* @return
*/
TaskHandle_t Agent::getTask() {
	return xHandle;
}

/***
 * Start the task
 * @param priority - Priority to apply to process
 * @return
 */
bool Agent::start(const char *name, UBaseType_t priority) {
	BaseType_t res;

	if (strlen(name) >= MAX_NAME_LEN){
		memcpy(pName, name, MAX_NAME_LEN);
		pName[MAX_NAME_LEN-1]=0;
	} else {
		strcpy(pName, name);
	}
	
	res = xTaskCreate(
		Agent::vTask,       /* Function that implements the task. */
		pName,   						/* Text name for the task. */
		getMaxStackSize(),	/* Stack size in words, not bytes. */
		( void * ) this,		/* Parameter passed into the task. */
		priority,						/* Priority at which the task is created. */
		&xHandle
	);

	return (res == pdPASS);
}

/***
 * Internal function used by FreeRTOS to run the task
 * @param pvParameters
 */
 void Agent::vTask( void * pvParameters ) {
	 Agent *task = (Agent *) pvParameters;
	 if (task != NULL) {
		 task->run();
	 }
 }

/***
* Return core that code is running on
* @return 0 or 1
*/
uint8_t Agent::getCore(){
	return (uint8_t)sio_hw->cpuid;
}
