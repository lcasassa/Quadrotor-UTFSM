#include <libopencm3/stm32/systick.h>
#include "systick.h"

u32 systick_counter = 0;
volatile u8 systick_flag = 0;

void systick_setup(void)
{
        /* 72MHz / 8 => 9000000 counts per second. */
        systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);

        /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
        systick_set_reload(9000);

        systick_interrupt_enable();

        /* Start counting. */
        systick_counter_enable();
}

void systick_handler(void)
{
        systick_counter++;
        systick_flag=1;
}

