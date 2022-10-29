/***
 * Demo program to use FreeRTOS SMP on Both Cores
 * Blink on Core 0 to GPIO 15
 * Blink on Core 1 to GPIO 16
 * Counter showing on GPIO 18 to 21, using Core 1
 * Instructions sent to Counter from Main Task on Core 0
 * Jon Durrant
 * 15-Aug-2022
 */

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>
#include "hardware/structs/sio.h"

#include "BlinkAgent.h"
#include "CounterAgent.h"


// Standard Task priority
#define TASK_PRIORITY ( tskIDLE_PRIORITY + 1UL )

// GPIOs to use for blinking the LEDs
#define LED_BLINK_0	15
#define LED_BLINK_1	16

// GPIOs to use for the counter
#define LED1	18
#define LED2	19
#define LED3	20
#define LED4	21


/***
 * Main task to blink external LED
 * @param params - unused
 */
void mainTask(void *params) {
	BlinkAgent blink{LED_BLINK_0};
	BlinkAgent blink1{LED_BLINK_1};
	CounterAgent counter{LED1, LED2, LED3, LED4};

	printf("Main task started\n");

	blink.start("Blink 0", TASK_PRIORITY);
	blink1.start("Blink 1", TASK_PRIORITY);
	counter.start("Counter", TASK_PRIORITY);

	// Bind to CORE 1
	UBaseType_t coreMask = 0x2;
	vTaskCoreAffinitySet( blink1.getTask(), coreMask );
	vTaskCoreAffinitySet( counter.getTask(), coreMask );

	// Bind to CORE 0
	coreMask = 0x1;
	vTaskCoreAffinitySet( blink.getTask(), coreMask );

	while (true) { // Loop forever
		printf("Main on Core %ld\n", sio_hw->cpuid);

		uint8_t r = rand() &0xF;
		counter.on(r);
		vTaskDelay(3000);
	}
}

/***
 * Launch the tasks and scheduler
 */
void vLaunch() 
{
	//Start blink task
	TaskHandle_t task;
	xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

	/* Start the tasks and timer running. */
	vTaskStartScheduler();
}

/***
 * Main
 * @return
 */
int main()
{
	//Setup serial over USB and give a few seconds to settle before we start
	stdio_init_all();
	sleep_ms(2000);
	printf("GO\n");

	//Start tasks and scheduler

	const char *rtos_name;
#if ( portSUPPORT_SMP == 1 )
	rtos_name = "FreeRTOS SMP";
#else
	rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
	printf("%s on both cores:\n", rtos_name);
#else
	printf("Starting %s on core 0:\n", rtos_name);
#endif

	vLaunch();

	return 0;
}
