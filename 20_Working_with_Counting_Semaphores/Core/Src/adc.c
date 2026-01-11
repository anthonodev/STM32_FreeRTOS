#include "stm32f4xx_hal.h"
#include  "adc.h"

void adc_init(void)
{
	//Enable clock GPIOA
	RCC->AHB1ENR |= (1U<<0);
	//Enable clock ADC1
	RCC->APB2ENR |= (1U<<8);
	//Set PA1 Analog
	GPIOA->MODER |= 0xC;
	//SW trigger
	ADC1->CR2 = 0;
	//Conversion sequence starts at ch 1
	ADC1->SQR3 = 1;
	//conversion sequence length 1
	ADC1->SQR1 = 0;
	//Enable ADC1
	ADC1->CR2 |= 1;
}

uint32_t read_analog_sensor(void)
{
	//Start conversion
	ADC1->CR2 |=(1U<<30);
	//Wait for converison to complete
	while(!(ADC1->SR & 2)){}
	//Return res
	return ADC1->DR;
}
