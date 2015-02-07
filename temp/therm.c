/**
 * Usage implies the DS18B20 sensor with w1-therm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "therm.h"

#define TRUE 1
#define FALSE 0

#define NOT_FOUND -1

#define MAX_BUFLEN 120
#define IDENTIFIER_SIZE 26

#define TEMP_LEN 5
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

char * removeNewline(char *string_with_newline)
{
	char *pos;
	if ((pos = strchr(string_with_newline, '\n')) != NULL) {
	    *pos = '\0';
	}

	return string_with_newline;
}

char * extractIdentifier(char *str_with_identifier)
{
	char str[IDENTIFIER_SIZE + 1];
	char *str_new = str;
	strncpy(str_new, str_with_identifier, IDENTIFIER_SIZE);
	str[IDENTIFIER_SIZE + 1] = '\0';
	return str_new;
}

int extractReading(char * str_with_reading)
{
	int pos;
	if ((pos = strpos(str_with_reading, "t=")) == NOT_FOUND) {
		printf("Failed to read temperature from: %s", str_with_reading);
		return BAD_READING;
	}

	char reading[TEMP_LEN + 1];
	char *reading_ptr = reading;
	strncpy(reading_ptr, str_with_reading + (pos + 2), TEMP_LEN);
	reading[TEMP_LEN + 1] = '\0';

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

	/*
		if the first line contains YES, then there will be a second line
		with an identifier and a reading in it
	 */
	char success_line[MAX_BUFLEN];
	fgets(success_line, sizeof(success_line), fp);

	if (strpos(success_line, "YES") == NOT_FOUND) {
		/* intentionally out-of-range value for the sensor */
		reading->temperature = BAD_READING;
	} else {
		char temperature_line[MAX_BUFLEN];
		fgets(temperature_line, sizeof(temperature_line), fp);
		reading->temperature = extractReading(removeNewline(temperature_line));
	}

	/* read temp and identifier from file innem */
	reading->identifier = extractIdentifier(removeNewline(success_line));

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
