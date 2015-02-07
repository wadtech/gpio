#include <stdio.h>
#include <stdlib.h>

#include "therm.h"

#define TRUE 1
#define FALSE 0

int thermEnabled(char *location)
{
	FILE *fp;

	fp = fopen(location, "r");
	if (fp == NULL) {
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

reading_t * thermRead(char *location)
{
	reading_t *reading = malloc(sizeof(reading_t));
	if (reading == NULL) {
		return NULL;
	}

	/* read temp and identifier from file innem */
	reading->identifier = "dunno\0";
	reading->temperature = 12345;

	return reading;
}

float thermCelcius(reading_t *raw)
{
	float c = raw->temperature / 1000;
	return c;
}

float thermFarenheit(reading_t *raw)
{
	int c = raw->temperature;
	float f = (((c * 9) / 5) + 32) / 1000;
	return f;
}
