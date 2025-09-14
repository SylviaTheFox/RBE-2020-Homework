// import standard library
#include "pico/stdlib.h"

// Representation of each digit in the counter
int CODE[10][8] = {{1, 0, 1, 1, 1, 0, 1, 1},  // 0
				   {1, 0, 0, 0, 0, 0, 1, 0},  // 1
				   {0, 0, 1, 1, 0, 1, 1, 1},  // 2
				   {1, 0, 0, 1, 0, 1, 1, 1},  // 3
				   {1, 0, 0, 0, 1, 1, 1, 0},  // 4
				   {1, 0, 0, 1, 1, 1, 0, 1},  // 5
				   {1, 0, 1, 1, 1, 1, 0, 1},  // 6
				   {1, 0, 0, 0, 0, 0, 1, 1},  // 7
				   {1, 0, 1, 1, 1, 1, 1, 1},  // 8
				   {1, 0, 0, 1, 1, 1, 1, 1}}; // 9
// what number we are at
int count = 0;
struct repeating_timer timer;

void moveCount(uint gpio, uint32_t events)
{
	count++;
	if (count == 10)
	{
		count = 0;
	}
	for (int i = 12; i <= 19; i++)
	{
		gpio_put(i, 1 - CODE[count][i - 12]);
	}
}

void timerCount()
{
	count++;
	if (count == 10)
	{
		count = 0;
	}
	for (int i = 12; i <= 19; i++)
	{
		gpio_put(i, 1 - CODE[count][i - 12]);
	}
}

void toggleTimer()
{
	if (timer.delay_us == 1000000)
	{
		cancel_repeating_timer(&timer);
		add_repeating_timer_ms(250, moveCount, NULL, &timer);
	}
	else
	{
		cancel_repeating_timer(&timer);
		add_repeating_timer_ms(1000, moveCount, NULL, &timer);
	}
}

int main()
{
	// initializes standard IO pins
	stdio_init_all();

	// initialize each GPIO pin as an output pin and set it to high
	for (int i = 12; i <= 19; i++)
	{
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
		gpio_put(i, 1 - CODE[count][i - 12]);
	}
	int BUTTON = 0;
	// button gpio
	gpio_init(BUTTON);
	gpio_set_dir(BUTTON, GPIO_IN);
	gpio_pull_up(BUTTON);

	// timer
	gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_RISE, 1, toggleTimer);
	add_repeating_timer_ms(1000, timerCount, NULL, &timer);

	while (true)
	{
		sleep_ms(1000);
	}

	return -1;
}
