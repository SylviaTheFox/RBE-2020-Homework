// import standard library
#include "pico/stdlib.h"

int main()
{
	// initializes standard IO pins
	stdio_init_all();

	// initialize each GPIO pin as an output pin and set it to high
	for (int i = 12; i <= 19; i++)
	{
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
		gpio_put(i, 1);
	}

	int index = 12; // which LED are we turning off
	while (true)
	{
		gpio_put(index, 1); // Turn off an LED
		if (index == 19)
		{
			index = 12; // Wraparound to 13
		}
		else
		{
			index++; // Increment counter
		}
		gpio_put(index, 0); // Turn on the next LED
		sleep_ms(1000);
	}

	return -1;
}
