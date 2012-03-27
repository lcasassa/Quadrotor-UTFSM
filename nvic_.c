#include <libopencm3/stm32/nvic.h>
#include "nvic_.h"

void nvic_setup()
{

	nvic_enable_irq(NVIC_HARD_FAULT_IRQ);
	nvic_set_priority(NVIC_HARD_FAULT_IRQ, 0);

//	nvic_enable_irq(NVIC_EXTI2_IRQ);
//	nvic_set_priority(NVIC_EXTI2_IRQ, 1);

	nvic_enable_irq(NVIC_TIM3_IRQ);
	nvic_set_priority(NVIC_TIM3_IRQ, 1);

	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_set_priority(NVIC_EXTI9_5_IRQ, 2);

	nvic_enable_irq(NVIC_EXTI15_10_IRQ);
	nvic_set_priority(NVIC_EXTI15_10_IRQ, 2);

	nvic_enable_irq(NVIC_USART1_IRQ);
	nvic_set_priority(NVIC_USART1_IRQ, 3);

	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 4);

}

