#include <stdio.h>
#include <wiringPi.h>

#define LED_1 0

int main (void)
{
	if (wiringPiSetup() == -1) {
		return 1;
	}

	printf("Raspberry Pi Blinking...\n");

	pinMode(LED_1, OUTPUT);

	for(;;) {
		digitalWrite(LED_1, HIGH);
		delay(500);
		digitalWrite(LED_1, LOW);
		delay(500);
	}

	return 0;
}

