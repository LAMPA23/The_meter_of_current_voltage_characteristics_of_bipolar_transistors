#include <stdio.h>
#include <stdlib.h>

#define Vref_V 2.935
#define Vref_mV 2935

#define Volt

// *** *** *** *** *** *** *** *** *** Functions List *** *** *** *** *** *** *** *** ***


// *** *** *** CLOCK functions *** *** ***
void clock_conf(void);


// *** *** *** UART functions *** *** ***
void init_UART2(void);
void UART_send_char(char ch);
void UART_send_string(char* s);
void UART_send_int(int i);


// *** *** *** LED functions *** *** ***
void init_GPIOA_out(int bit);
void init_GPIOD_out(int bit);
void switch_GPIOD_out(int bit);
void init_GPIOA_in(int bit);


// *** *** *** Delay functions *** *** ***
void delay(unsigned int i);
int num_of_dec(int num);
int power(int num, int s);
int get_dec(int num, int dec);
int float_to_int(float f);
void UART_send_adc(int num);

// *** *** *** DAC functions *** *** ***
void init_DAC(void);


// *** *** *** ADC functions *** *** ***
void init_ADC(void);
void ADC_conv(void);



// *** *** *** *** *** *** *** *** *** Registers List *** *** *** *** *** *** *** *** *** 


// *** *** *** RCC
#define RCC_base 0x40023800
#define RCC_CFGR_reg 0x08
#define AHB1ENR_reg 0x30
#define APB1ENR_reg 0x40
#define APB2ENR_reg 0x44


// *** *** *** GPIO
#define GPIOA_base 0x40020000
#define GPIOB_base 0x40020400
#define GPIOC_base 0x40020800
#define GPIOD_base 0x40020C00
#define MODER_reg 0x00
#define OSPEEDR_reg 0x08
#define IDR_reg 0x10
#define ODR_reg 0x14
#define BSRR_reg 0x18
#define AFRL_reg 0x20
#define AFRH_reg 0x24


// *** *** *** DAC
#define DAC_base 0x40007400
#define DAC_CR_reg 0x00
#define DAC_DHR12R1_reg 0x08
#define DAC_DHR12R2_reg 0x14


// *** *** *** USART2
#define USART2_base 0x40004400
#define USART_SR_reg 0x00
#define USART_DR_reg 0x04
#define USART_BRR_reg 0x08
#define USART_CR1_reg 0x0C

// *** *** *** ADC
#define ADC1_base 0x40012000
#define ADC_SR_reg 0x00
#define ADC_CR1_reg 0x04
#define ADC_CR2_reg 0x08
#define ADC_SMPR1_reg 0x0C
#define ADC_SMPR2_reg 0x10

#define ADC_SQR1_reg 0x2C
#define ADC_SQR2_reg 0x30
#define ADC_SQR3_reg 0x34
#define ADC_JSQR_reg 0x38

#define ADC_JDR1_reg 0x3C
#define ADC_JDR2_reg 0x40
#define ADC_JDR3_reg 0x44
#define ADC_JDR4_reg 0x48
#define ADC_DR_reg 0x4C


