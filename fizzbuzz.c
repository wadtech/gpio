#include <stdio.h>
#include <wiringPi.h>

#define LED_1 0
#define LED_2 2

#define STEP_DELAY 250

int main (void)
{
	if (wiringPiSetup() == -1) {
		return 1;
	}

	printf("Raspberry Pi Blinking...\n");

	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);

	for(int i = 0; i < 250; i++) {
		if (i % 3 == 0 && i % 5 == 0) {
			printf("fizzbuzz\n");
			digitalWrite(LED_1, HIGH);
			digitalWrite(LED_2, HIGH);
		} else if (i % 3 == 0) {
			printf("fizz\n");
			digitalWrite(LED_1, HIGH);
		} else if (i % 5 == 0) {
			printf("buzz\n");
			digitalWrite(LED_2, HIGH);
		} else {
			printf("%d\n", i);
		}

		delay(STEP_DELAY);

		digitalWrite(LED_1, LOW);
		digitalWrite(LED_2, LOW);
	}

	return 0;
}

