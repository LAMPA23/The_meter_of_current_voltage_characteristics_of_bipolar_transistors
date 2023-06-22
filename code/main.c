#include "inc.h"

int main(void)
{
	clock_conf();
	init_UART2();
	init_DAC();
	init_ADC();

	UART_send_string("\n\n\nSTART:\n\r(V1:V2) U1|U2|U3|U4\n\r");
	
	for(unsigned int i = 0; i < 4095; i += 400)
	{
		*(unsigned int*)(DAC_base + DAC_DHR12R2_reg) = i;
		for(unsigned int j = 0; j < 4095; j += 400)
		{
			*(unsigned int*)(DAC_base + DAC_DHR12R1_reg) = j;
			
			UART_send_string("(");
			UART_send_adc(*(int*)(DAC_base + DAC_DHR12R2_reg));
			UART_send_string(":");
			UART_send_adc(*(int*)(DAC_base + DAC_DHR12R1_reg));
			UART_send_string(") ");
			
			ADC_conv();
		}
	}

	UART_send_string("STOP\n\r");
	
	while(1);
}

