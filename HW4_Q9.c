#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

int volts;
const int CODE[10][7] = {
	{0, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 1, 1, 0},
	{1, 0, 1, 1, 0, 1, 1},
	{1, 0, 0, 1, 1, 1, 1},
	{1, 1, 0, 0, 1, 1, 0},
	{1, 1, 0, 1, 1, 0, 1},
	{1, 1, 1, 1, 1, 0, 1},
	{0, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1},
	{1, 1, 0, 1, 1, 1, 1}};
struct repeating_timer timer;
struct repeating_timer mainTimer;

float adcToVolt(float adc)
{
	return adc * 3.3f / (1 << 12);
}

void putNum(int num) // print to the 7-segment
{
	for (int i = 13; i < 20; i++)
	{
		gpio_put(i, 1 - CODE[num][i - 13]);
	}
}

bool displayThousandths() // for each of these we take the most significant digit then mod to remove it
{
	volts = volts % 10;
	int value = volts;

	gpio_put(12, 1);
	gpio_put(11, 0);
	if (isPWM)
	{
		gpio_put(20, 1);
	}

	putNum(value);
	return false;
}

bool displayHundredths()
{
	volts = volts % 100;
	int value = volts / 10;

	gpio_put(11, 1);
	gpio_put(10, 0);
	if (isPWM)
	{
		gpio_put(20, 0);
	}

	putNum(value);

	add_repeating_timer_ms(2, displayThousandths, NULL, &timer);
	return false;
}

bool displayTenths()
{
	volts = volts % 1000;
	int value = volts / 100;

	gpio_put(10, 1);
	gpio_put(9, 0);
	if (!isPWM)
	{
		gpio_put(20, 1);
	}
	putNum(value);

	add_repeating_timer_ms(2, displayHundredths, NULL, &timer);
	return false;
}

bool displayOnes()
{
	int value = volts / 1000;

	gpio_put(9, 1);
	gpio_put(12, 0);
	if (!isPWM)
	{
		gpio_put(20, 0);
	}
	putNum(value);

	add_repeating_timer_ms(2, displayTenths, NULL, &timer);
	return true;
}

bool displayVoltage()
{
	// adc to volts
	volts = (int)(adcToVolt(adc_read()) * 1000);
	displayOnes();
	return true;
}

int main()
{
	// initialize
	stdio_init_all();
	adc_init();
	adc_gpio_init(26);
	adc_select_input(0);

	for (int i = 9; i < 21; i++)
	{
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
		gpio_put(i, 0);
	}
	// voltmeter
	add_repeating_timer_ms(10, displayVoltage, NULL, &mainTimer);

	// ending
	while (true)
	{
		sleep_ms(1000);
	}
}
