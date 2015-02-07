#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

#include "therm.h"

#define TRUE 1
#define FALSE 0

#define LED_1 0
#define LED_2 2

#define THERM_LOCATION "/sys/bus/w1/devices/28-00141186abff/w1_slave"

int setUp()
{
	if (wiringPiSetup() == -1) {
		return FALSE;
	}

	if (thermEnabled(THERM_LOCATION) == FALSE) {
		return FALSE;
	}

	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);

	return TRUE;
}

void cleanUp()
{
	digitalWrite(LED_1, LOW);
	digitalWrite(LED_2, LOW);
}

/* Only really have this because signal needs a function to call */
void cleanUpAndExit()
{
	printf("Exiting...\n");
	cleanUp();
	exit(0);
}

int main(void)
{
	if (setUp() == FALSE) {
		printf("Failed to run setup.\n");
		return 1;
	}

	/* Handles ctrl+c so we get cleanup, and importantly switch off the
	   "running" LED */
    signal(SIGINT, cleanUpAndExit);

	/* light this LED to show the program is running. */
	digitalWrite(LED_1, HIGH);

	float degrees_c;

	for(;;) {
		/* flash the working LED to show that we're
		   taking a temp and doing some work */
		digitalWrite(LED_2, HIGH);

		reading_t *raw = thermRead(THERM_LOCATION);
		if (raw == NULL) {
			// what a state, failed malloc or something probably
			printf("Failed to read from the sensor\n");
			cleanUp();
			return 1;
		}

		degrees_c = thermCelcius(raw);

		// probably write to a file or something here...
		printf("%s: %2.0fC\n", raw->identifier, degrees_c);

		digitalWrite(LED_2, LOW);
		delay(10000);
	}

	cleanUpAndExit();
	return 0;
}
