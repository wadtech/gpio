#ifndef H_THERM
#define H_THERM

typedef struct {
	int temperature; /* the temperature represented as an int */
} reading_t;

/* Looks for the device directory using the serial provided */
int thermEnabled(char *serial);

/* capture the current reading in a struct */
reading_t * thermRead(char *serial);

/* convert the raw temperature value to its
   celcius or farenheit value as a float. */
float thermCelcius(reading_t *raw);
float thermFarenheit(reading_t *raw);

#endif
