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

	int code[10][8] = {{1, 0, 1, 1, 1, 0, 1, 1},  // 0
					   {1, 0, 0, 0, 0, 0, 1, 0},  // 1
					   {0, 0, 1, 1, 0, 1, 1, 1},  // 2
					   {1, 0, 0, 1, 0, 1, 1, 1},  // 3
					   {1, 0, 0, 0, 1, 1, 1, 0},  // 4
					   {1, 0, 0, 1, 1, 1, 0, 1},  // 5
					   {1, 0, 1, 1, 1, 1, 0, 1},  // 6
					   {1, 0, 0, 0, 0, 0, 1, 1},  // 7
					   {1, 0, 1, 1, 1, 1, 1, 1},  // 8
					   {1, 0, 0, 1, 1, 1, 1, 1}}; // 9
	int index = 0;
	while (true)
	{
		for (int i = 12; i <= 19; i++)
		{
			gpio_put(i, 1 - code[index][i - 12]); // set each pin to the appropriet output defined in code[][]
		}
		index++; // go to the next number
		if (index == 10)
		{ // wrap around
			index = 0;
		}
		sleep_ms(1000); // wait
	}

	return -1;
}
