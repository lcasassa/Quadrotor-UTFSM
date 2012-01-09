#include <libopencm3/stm32/nvic.h>
//#include <libopencm3/stm32/f1/scb.h>
#include "nvic_.h"

void nvic_setup()
{
//#define NVIC_NMI_IRQ                    -14


//SCB_SHCSR = 0x00007000;

	nvic_enable_irq(NVIC_HARD_FAULT_IRQ);
	nvic_set_priority(NVIC_HARD_FAULT_IRQ, 0);
/*
	nvic_enable_irq(NVIC_MEM_MANAGE_IRQ);
	nvic_set_priority(NVIC_MEM_MANAGE_IRQ, 0);

	nvic_enable_irq(NVIC_BUS_FAULT_IRQ);
	nvic_set_priority(NVIC_BUS_FAULT_IRQ, 0);

	nvic_enable_irq(NVIC_USAGE_FAULT_IRQ);
	nvic_set_priority(NVIC_USAGE_FAULT_IRQ, 0);
*/
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_set_priority(NVIC_EXTI9_5_IRQ, 1);

	nvic_enable_irq(NVIC_EXTI15_10_IRQ);
	nvic_set_priority(NVIC_EXTI15_10_IRQ, 1);

	nvic_enable_irq(NVIC_USART1_IRQ);
	nvic_set_priority(NVIC_USART1_IRQ, 4);

	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 3);

}
