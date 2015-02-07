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
#define OUTPUT_FILE "./readings.txt"

#define POLL_DELAY 30000 /* 30 seconds */

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
	printf("Exiting...\n");
	digitalWrite(LED_1, LOW);
	digitalWrite(LED_2, LOW);
}

/* Only really have this because signal needs a function to call */
void cleanUpAndExit()
{
	cleanUp();
	exit(0);
}

/* Only really have this because signal needs a function to call */
void cleanUpAndError()
{
	cleanUp();
	exit(1);
}

int writeResult(char *filepath, reading_t *reading)
{
	FILE *of;
	of = fopen(filepath, "a");
	if (of == NULL) {
		return FALSE;
	}

	int written = fprintf(of, "%2.0f\n", thermCelcius(reading));
	if (written == 0) {
		fclose(of);
		return FALSE;
	}

	fclose(of);
	return TRUE;
}

int main(void)
{
	if (setUp() == FALSE) {
		printf("Failed to run setup.\n");
		return 1;
	}

	/* Handles ctrl+c so we get cleanup, and importantly switch off the
	   "running" LED */
    signal(SIGINT, cleanUpAndError);

	/* light this LED to show the program is running. */
	digitalWrite(LED_1, HIGH);

	for(;;) {
		/* flash the working LED to show that we're
		   taking a temp and doing some work */
		digitalWrite(LED_2, HIGH);

		reading_t *reading = thermRead(THERM_LOCATION);
		if (reading == NULL) {
			/* what a state, failed malloc or something probably */
			printf("Failed to read from the sensor\n");
			cleanUpAndExit();
		}

		/* probably write to a file or something here... */
		writeResult(OUTPUT_FILE, reading);
		printf("%2.0f°C\n", thermCelcius(reading));

		digitalWrite(LED_2, LOW);

		/* We're done with the reading struct, so free it up. */
		free(reading);
		delay(POLL_DELAY);
	}

	cleanUpAndExit();
}
