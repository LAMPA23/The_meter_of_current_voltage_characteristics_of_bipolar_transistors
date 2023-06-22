#include "inc.h"


// *** *** *** CLOCK functions *** *** *** --------------------------------------------------
void clock_conf(void)
{
	*(unsigned int*)(RCC_base + RCC_CFGR_reg) &= ~(3U << 0);
	*(unsigned int*)(RCC_base + RCC_CFGR_reg) &= ~(7U << 12);
	*(unsigned int*)(RCC_base + RCC_CFGR_reg) &= ~(15U << 7); 
}



// *** *** *** UART functions *** *** *** --------------------------------------------------
void init_UART2(void) 
{
	*(unsigned int*)(RCC_base + APB1ENR_reg) |= 1 << 17; // USART2 clock
	*(unsigned int*)(RCC_base + AHB1ENR_reg) |= 1 << 0; // GPIOA  clock
	
	// PA2 TX config
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (1U << ((2 * 2) + 1));
	*(unsigned int*)(GPIOA_base + MODER_reg) &= ~(1U << (2 * 2));
	*(unsigned int*)(GPIOA_base + OSPEEDR_reg) |= (1U << ((5 * 5) + 1));
	*(unsigned int*)(GPIOA_base + OSPEEDR_reg) &= ~(1U << (5 * 5)); 
	*(unsigned int*)(GPIOA_base + AFRL_reg) |= 7 << (4 * 2);
	
	// PA3 RX config
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (1U << ((3 * 2) + 1));
	*(unsigned int*)(GPIOA_base + MODER_reg) &= ~(1U << (3 * 2));
	*(unsigned int*)(GPIOA_base + OSPEEDR_reg) |= (1U << ((3 * 2) + 1));
	*(unsigned int*)(GPIOA_base + OSPEEDR_reg) &= ~(1U << (3 * 2)); 
	*(unsigned int*)(GPIOA_base + AFRL_reg) |= 7 << (4 * 3);
	
	// USART1 config
	*(unsigned int*)(USART2_base + USART_BRR_reg) = (0x68 << 4) + 0x2;
	*(unsigned int*)(USART2_base + USART_CR1_reg) |= 1 << 3; // Увімкнення передавача
	*(unsigned int*)(USART2_base + USART_CR1_reg) &= ~(1U << 10); // Parity disabled
	*(unsigned int*)(USART2_base + USART_CR1_reg) |= 1 << 13; // Увімкнення UART
	*(unsigned int*)(USART2_base + USART_CR1_reg) &= ~(1U << 15);  // OVER8 disabled
}

void UART_send_adc(int num)
{
	#ifdef Volt
		int buf = (num * Vref_mV / 4095) / 1000;
		UART_send_int(buf);
		UART_send_string(".");
		buf = (num * Vref_mV / 4095) - (buf * 1000); 
		UART_send_int(buf);
		return;
	#endif
	UART_send_int(num);
	
}

void UART_send_char(char ch)
{
  while (!(*(unsigned int*)(USART2_base + USART_SR_reg) & (1 << 7)));// Очікування, доки передавач не буде готовим
  *(unsigned int*)(USART2_base + USART_DR_reg) = ch;  // Відправка символу
}

void UART_send_string(char* message)
{
	for(unsigned int i = 0; message[i] != '\0'; i++)
	{
		UART_send_char(message[i]);
	}
}

void UART_send_int(int i)
{
	if(i < 0) 
	{
		UART_send_char('-');
		i = -i;
	}
	int length = num_of_dec(i); // num of dec
	for(int j = length; j > 0; j--)
	{
		UART_send_char(48 + (char)get_dec(i, j));
	}
	//UART_send_string("\n\r");
}

int get_dec(int num, int dec)
{
	return (num % power(10, dec) - num % power(10, dec - 1)) / power(10, dec - 1); 
}

int num_of_dec(int num)
{
	if(num < 0) num = -num;
	for(int i = 1; i < 9; i++)
	{
		if(power(10, i) > num) return i;
	}
	UART_send_string("Error num_of_dec\n\r");
	return NULL;
}

int power(int num, int s)
{
	if(s < 0 || s > 10 || num < 0) 
	{
		UART_send_string("Error pow\n\r");
		return NULL;
	}
	int ans = 1;
	for(int i = 0; i < s; i++)
	{
		ans *= num;
	}
	return ans;
}



// *** *** *** GPIO functions *** *** *** --------------------------------------------------

void init_GPIOA_out(int bit)
{
	if(bit > 15 || bit < 0)
	{
		UART_send_string("Error init_GPIOA_in\n\r");
		return;
	}
	*(unsigned int*)(RCC_base + AHB1ENR_reg) |= (1 << 3);
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (1U << (bit * 2));
	*(unsigned int*)(GPIOA_base + MODER_reg) &= ~(1U << ((bit * 2) + 1));
	*(unsigned int*)(GPIOA_base + BSRR_reg) |= (1 << bit);
}

void init_GPIOD_out(int bit)
{
	if(bit > 15 || bit < 0)
	{
		UART_send_string("Error init_GPIOA_in\n\r");
		return;
	}
	*(unsigned int*)(RCC_base + AHB1ENR_reg) |= (1 << 3);
	*(unsigned int*)(GPIOD_base + MODER_reg) |= (1U << (bit * 2));
	*(unsigned int*)(GPIOD_base + MODER_reg) &= ~(1U << ((bit * 2) + 1));
	*(unsigned int*)(GPIOD_base + BSRR_reg) |= (1 << bit);
}

void switch_GPIOD_out(int bit)
{
	if(bit > 15 || bit < 0)
	{
		UART_send_string("Error init_GPIOA_in\n\r");
		return;
	}
	if(*(unsigned int*)(GPIOD_base + IDR_reg) & (1 << bit))
		*(unsigned int*)(GPIOD_base + BSRR_reg) = (1 << (bit + 16));
	else
		*(unsigned int*)(GPIOD_base + BSRR_reg) = (1 << bit);
}

void init_GPIOA_in(int bit)
{
	if(bit > 15 || bit < 0)
	{
		UART_send_string("Error init_GPIOA_in\n\r");
		return;
	}
	*(unsigned int*)(RCC_base + AHB1ENR_reg) |= (1 << 3);
	*(unsigned int*)(GPIOA_base + MODER_reg) &= ~(1U << (bit * 2));
	*(unsigned int*)(GPIOA_base + MODER_reg) &= ~(1U << ((bit * 2) + 1));
}


// *** *** *** Delay functions *** *** *** --------------------------------------------------
void delay(unsigned int i)
{
	for(volatile unsigned int time = 0; time <= i; time++);
}



// *** *** *** DAC functions *** *** *** --------------------------------------------------
void init_DAC(void)
{
	*(unsigned int*)(RCC_base + AHB1ENR_reg) |= 1; 					// clock GPIOA
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (3U << (4 * 2));	// PA4 - analog mode
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (3U << (5 * 2));	// PA5 - analog mode
	
	*(unsigned int*)(RCC_base + APB1ENR_reg) |= 1 << 29;	// clock DAC							
	*(unsigned int*)(DAC_base + DAC_CR_reg) |= (1U << 0);	// enable chanel 1
	*(unsigned int*)(DAC_base + DAC_CR_reg) |= (1U << 16);	// enable chanel 2
	
	*(unsigned int*)(DAC_base + DAC_DHR12R1_reg) = (unsigned int)(4095 * 0 / Vref_V);	// PA4
	*(unsigned int*)(DAC_base + DAC_DHR12R2_reg) = (unsigned int)(4096 * 0 / Vref_V);	// PA5
}

void init_ADC(void)
{
	// clear CR
	*(unsigned int*)(ADC1_base + ADC_CR1_reg) = 0;
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) = 0;
	
	// clock
	//*(unsigned int*)(RCC_base + AHB1ENR_reg) |= 1 << 0; // PA
	*(unsigned int*)(RCC_base + AHB1ENR_reg) |= 1 << 2; // PC
	*(unsigned int*)(RCC_base + APB2ENR_reg) |= 1 << 8; // ADC1
	
	// gpio analog mode
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (3U << (6 * 2)); //pa6 = ch6
	*(unsigned int*)(GPIOA_base + MODER_reg) |= (3U << (7 * 2)); //pa7 = ch7
	*(unsigned int*)(GPIOC_base + MODER_reg) |= (3U << (1 * 2)); //pc1 = ch11
	*(unsigned int*)(GPIOC_base + MODER_reg) |= (3U << (2 * 2)); //pc2 = ch12
		
	// sample for ch 11 is max(480)
	*(unsigned int*)(ADC1_base + ADC_SMPR1_reg) = 7 << (3 * 1);
	*(unsigned int*)(ADC1_base + ADC_SMPR1_reg) = 7 << (3 * 2);
	*(unsigned int*)(ADC1_base + ADC_SMPR2_reg) = 7 << (3 * 6);
	*(unsigned int*)(ADC1_base + ADC_SMPR2_reg) = 7 << (3 * 7);
	
	// injected sequence conf ch11 only
	*(unsigned int*)(ADC1_base + ADC_JSQR_reg) = 0;
	*(unsigned int*)(ADC1_base + ADC_JSQR_reg) |= 3 << 20;
	*(unsigned int*)(ADC1_base + ADC_JSQR_reg) |= 12 << 15;
	*(unsigned int*)(ADC1_base + ADC_JSQR_reg) |= 11 << 10;
	*(unsigned int*)(ADC1_base + ADC_JSQR_reg) |= 7 << 5;
	*(unsigned int*)(ADC1_base + ADC_JSQR_reg) |= 6 << 0;
	
	// JEXTEN
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) |= 1 << 20;
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) &= ~(1U << 21);
	
	// JEXTSEL
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) = 15 << 16;	

	// CONT
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) &= ~(1U << 1);
	
	// SCAN
	*(unsigned int*)(ADC1_base + ADC_CR1_reg) |= (1U << 8);
	
	// ADON
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) |= 1 << 0;
}

void ADC_conv(void)
{
	// JSWSTART
	*(unsigned int*)(ADC1_base + ADC_CR2_reg) |= 1 << 22;
	
	// wait EOC
	while(!(*(unsigned int*)(ADC1_base + ADC_SR_reg) & (1 << 2)));
	
	UART_send_adc(*(int*)(ADC1_base + ADC_JDR1_reg));
	UART_send_string("|");
	UART_send_adc(*(int*)(ADC1_base + ADC_JDR2_reg));
	UART_send_string("|");
	UART_send_adc(*(int*)(ADC1_base + ADC_JDR3_reg));
	UART_send_string("|");
	UART_send_adc(*(int*)(ADC1_base + ADC_JDR4_reg));	
	UART_send_string("\n\r");
	
	*(unsigned int*)(ADC1_base + ADC_SR_reg) &= ~(1U << 2); 
}


