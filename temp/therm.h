#ifndef H_THERM
#define H_THERM

/*
	Represents a reading from the sensor: 
		a hex identifier in a string, 
		the raw temperature reading in a 5-byte int
*/
typedef struct {
	char *identifier;
	int temperature;
} reading_t;

/* Looks for the device directory using the serial provided */
int thermEnabled(char *serial);

/* capture the current reading in a struct */
reading_t thermRead(char *serial);

/* convert the raw temperature value to it's celcius or farenheit value as a float */
float thermCelcius(reading_t *raw);
float thermFarenheit(reading_t *raw);

#endif
