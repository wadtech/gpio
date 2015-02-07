/* Usage implies the DS18B20 sensor with w1-therm

	modprobe w1-gpio
	modprobe w1-therm

 	cat /sys/bus/w1/devices/28-<serial>/w1_slave
		5b 01 55 00 7f ff 0c 10 3a : crc=3a YES
		5b 01 55 00 7f ff 0c 10 3a t=21687

	if the first line ends in YES then the second line will contain
	the temperature reading.

	The user of this code must provide the location to the w1_slave file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "therm.h"

#define TRUE 1
#define FALSE 0

#define NOT_FOUND -1

#define MAX_BUFLEN 120
/* so arbitrary, it's a hex value with spaces between each pair.
   i.e. FF FF FF FF FF FF FF FF FF  */
#define IDENTIFIER_LEN 26

/* more magic numbers, the temperature is 5 characters long from the sensor */
#define TEMPERATURE_LEN 5
#define BAD_READING -999 /* out-of-range for the sensor */

/* pinched from SO http://stackoverflow.com/a/7655509/771564 */
int strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p) {
      return p - haystack;
   }

   return NOT_FOUND;
}

int removeNewline(char *string_with_newline)
{
	char *pos;
	if ((pos = strchr(string_with_newline, '\n')) != NULL) {
	    *pos = '\0';
	}

	return TRUE;
}

int extractReading(char * str_with_reading)
{
	int pos;
	if ((pos = strpos(str_with_reading, "t=")) == NOT_FOUND) {
		printf("Failed to read temperature from: %s", str_with_reading);
		return BAD_READING;
	}

	char reading[TEMPERATURE_LEN + 1];
	char *reading_ptr = reading;

	strncpy(reading_ptr, str_with_reading + (pos + 2), TEMPERATURE_LEN);
	reading[TEMPERATURE_LEN + 1] = '\0';

	return atoi(reading);
}

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

	FILE *fp;

	fp = fopen(location, "r");
	if (fp == NULL) {
		return FALSE;
	}

	/* if the first line contains YES, then there will be a second line
	   with an identifier and a reading in it */
	char success_line[MAX_BUFLEN];
	fgets(success_line, sizeof(success_line), fp);
	removeNewline(success_line);

	if (strpos(success_line, "YES") == NOT_FOUND) {
		/* intentionally out-of-range value for the sensor */
		reading->temperature = BAD_READING;
	} else {
		char temperature_line[MAX_BUFLEN];
		fgets(temperature_line, sizeof(temperature_line), fp);
		removeNewline(temperature_line);
		reading->temperature = extractReading(temperature_line);
	}

	fclose(fp);
	return reading;
}

float thermCelcius(reading_t *raw)
{
	float c = (float) raw->temperature;
	return c / 1000; //move three decimal places to the right
}

float thermFarenheit(reading_t *raw)
{
	float c = (float) raw->temperature;
	c = c / 1000; // move three decimal places to the right
	return ((c * 9) / 5) + 32;
}
