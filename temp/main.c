#include <stdio.h>
#include <stdlib.h>
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

int main(void)
{
	if (setUp() == FALSE) {
		printf("Failed to run setup.\n");
		return 1;
	}

	digitalWrite(LED_1, HIGH); // light this LED to show the program is running.

	float degrees_c;
	float degrees_f;

	for(;;) {
		digitalWrite(LED_2, HIGH); // flash the working LED to show that we're taking a temp and doing something with it

		reading_t *raw = thermRead(THERM_LOCATION);
		if (raw == NULL) {
			//something is fucky
			printf("Failed to read from the sensor\n");
			cleanUp();
			return 1;
		}

		degrees_c = thermCelcius(raw);
		degrees_f = thermFarenheit(raw);

		// probably write to a file or something here...
		printf("current reading is: %2.3fC, %2.3fF\n", degrees_c, degrees_f);

		digitalWrite(LED_2, LOW);
		delay(10000);
	}

	cleanUp();
	return 0;
}
