#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int timeout = 26100;


void SetUpPins(uint trigPin, uint echoPin)
{
	gpio_init(trigPin);
	gpio_init(echoPin);
	gpio_set_dir(trigPin, GPIO_OUT);
	gpio_set_dir(echoPin, GPIO_IN);
}

uint64_t getPulse(uint trigPin, uint echoPin)
{
	gpio_put(trigPin, 1);
	sleep_us(10);
	gpio_put(trigPin,0);
	
	uint width = 0;
	while(gpio_get(echoPin) == 0)
	{
		tight_loop_contents();
	}
	while(gpio_get(echoPin) == 1)
	{
		width++;
		sleep_us(1);
		if(width > timeout)
			return 0;
	}
	return width;
}


int getCm(uint trigPin, uint echoPin)
{
	uint64_t pulseLength = getPulse(trigPin, echoPin);
	return pulseLength / 29 / 2;
}

int getInch(uint trigPin, uint echoPin)
{
	uint64_t pulseLength = getPulse(trigPin, echoPin);
	return (long)pulseLength / 74.f / 2.f;
}


uint trigPin = 2;
uint echoPin = 3;

//const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint LED_PIN = 16;
int main()
{
	stdio_init_all();
	SetUpPins(trigPin, echoPin);

		gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	while(true)
	{
		uint val = getCm(trigPin, echoPin);
		printf("\n %d cm",val);
		if ( val < 5){
			gpio_put(LED_PIN, 1);
		}
		else
		{
			gpio_put(LED_PIN, 0);
		}
		sleep_us(10000);
	}
}

