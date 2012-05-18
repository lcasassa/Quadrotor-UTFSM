#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/nvic.h>
//#include <libopencm3/stm32/f1/flash.h>
#include <libopencm3/stm32/f1/gpio.h>
//#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <stdio.h>
#include "i2c1.h"

#define TIMEOUT_VALUE 1000
void i2c1_setup() {

//	rcc_peripheral_reset(&RCC_APB1RSTR, RCC_APB1RSTR_I2C2RST);
//	rcc_peripheral_clear_reset(&RCC_APB1RSTR, RCC_APB1RSTR_I2C2RST);

        /* Enable GPIOB clock. */
        rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);

	/* Enable clocks for I2C2 and AFIO. */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_I2C1EN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN);

	// Reset i2c
	rcc_peripheral_reset(&RCC_APB1RSTR, RCC_APB1RSTR_I2C1RST);
	rcc_peripheral_clear_reset(&RCC_APB1RSTR, RCC_APB1RSTR_I2C1RST);

	i2c1_recover();

	/* Set alternate functions for the SCL and SDA pins of I2C2. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		      GPIO_I2C1_SCL | GPIO_I2C1_SDA);

	/* Disable the I2C before changing any configuration. */
	i2c_peripheral_disable(I2C1);

	/* APB1 is running at 36MHz. */
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);

	/* 400KHz - I2C Fast Mode */
//	i2c_set_fast_mode(I2C2);
	/* 100KHz - I2C Fast Mode */
	i2c_set_standard_mode(I2C1);

	/*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
	i2c_set_ccr(I2C1, 0x1e);

	/*
	 * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
	 * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
	 * Incremented by 1 -> 11.
	 */
	i2c_set_trise(I2C1, 0x0b);

	/*
	 * This is our slave address - needed only if we want to receive from
	 * other masters.
	 */
	i2c_set_own_7bit_slave_address(I2C1, 0x02);

	/* If everything is configured -> enable the peripheral. */
	i2c_peripheral_enable(I2C1);

}

int i2c1_write(char sla, char reg_addr, char value) {
        u32 reg32;
	u32 timeout;
//printf("I2C_CR1(I2C1): 0x%x I2C_SR1(I2C1): 0x%x I2C_SR2(I2C1): 0x%x\r\n", I2C_CR1(I2C1), I2C_SR1(I2C1), I2C_SR2(I2C1));
        /* Send START condition. */
        i2c_send_start(I2C1);
//printf("I2C_CR1(I2C1): 0x%x I2C_SR1(I2C1): 0x%x I2C_SR2(I2C1): 0x%x\r\n", I2C_CR1(I2C1), I2C_SR1(I2C1), I2C_SR2(I2C1));

        /* Waiting for START is send and switched to master mode. */
	timeout=TIMEOUT_VALUE;
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
                & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))) && timeout!=0) timeout--;

//	if(timeout==0) printf("I2C_CR1(I2C1): 0x%x I2C_SR1(I2C1): 0x%x I2C_SR2(I2C1): 0x%x\r\n", I2C_CR1(I2C1), I2C_SR1(I2C1), I2C_SR2(I2C1));
	if(timeout==0) { return -1; }

        /* Send destination address. */
        i2c_send_7bit_address(I2C1, sla, I2C_WRITE);

        /* Waiting for address is transferred. */
	timeout=TIMEOUT_VALUE;
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR) && timeout!=0) {
		timeout--;
/*		if(I2C_SR1(I2C2) & I2C_SR1_AF) {
			I2C_SR1(I2C2) &= ~I2C_SR1_AF;
			i2c_send_stop(I2C2);
			return -2;
		}
*/
	}
	if(timeout==0) { return -3; }


        /* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

        /* Sending the data. */
        i2c_send_data(I2C1, reg_addr); /* stts75 config register */

	timeout=TIMEOUT_VALUE;
        while (!(I2C_SR1(I2C1) & I2C_SR1_BTF) && timeout!=0) timeout--; /* Await ByteTransferedFlag. */
	if(timeout==0) { return -4; }

        /* Polarity reverse - LED glows if temp is below Tos/Thyst. */
        i2c_send_data(I2C1, value);

	timeout=TIMEOUT_VALUE;
        while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)) && timeout!=0) timeout--;
	if(timeout==0) { return -5; }

        /* Send STOP condition. */
        i2c_send_stop(I2C1);

	return 1;
}

// TODO len can only be 2 for now..
int i2c1_read(char sla, char reg_addr, char * value, char len) {
	u32 reg32;
	u32 timeout;

        /* Send START condition. */
        i2c_send_start(I2C1);

        /* Waiting for START is send and switched to master mode. */
	timeout=TIMEOUT_VALUE;
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB) & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))) && timeout!=0) timeout--;
	if(timeout==0) { return -1; }

        /* Say to what address we want to talk to. */
        /* Yes, WRITE is correct - for selecting register in STTS75. */
        i2c_send_7bit_address(I2C1, sla, I2C_WRITE);


        /* Waiting for address is transferred. */
	timeout=TIMEOUT_VALUE;
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR) && timeout!=0) {
		timeout--;
/*
		if(I2C_SR1(I2C2) & I2C_SR1_AF) {
			I2C_SR1(I2C2) &= ~I2C_SR1_AF;
			i2c_send_stop(I2C2);
			return -2;
		}
*/
	}
	if(timeout==0) { return -3; }

        /* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

        i2c_send_data(I2C1, reg_addr); /* temperature register */
	timeout=TIMEOUT_VALUE;
        while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)) && timeout!=0) timeout--;
	if(timeout==0) { return -4; }

        /*
         * Now we transferred that we want to ACCESS the temperature register.
         * Now we send another START condition (repeated START) and then
         * transfer the destination but with flag READ.
         */

        /* Send START condition. */
        i2c_send_start(I2C1);

        /* Waiting for START is send and switched to master mode. */
	timeout=TIMEOUT_VALUE;
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
                & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))) && timeout!=0) timeout--;
	if(timeout==0) { return -5; }

        /* Say to what address we want to talk to. */
        i2c_send_7bit_address(I2C1, sla, I2C_READ);
        /* 2-byte receive is a special case. See datasheet POS bit. */
        I2C_CR1(I2C1) |= (I2C_CR1_POS | I2C_CR1_ACK);

        /* Waiting for address is transferred. */
	timeout=TIMEOUT_VALUE;
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR) && timeout!=0) {
		timeout--;
/*
		if(I2C_SR1(I2C2) & I2C_SR1_AF) {
			I2C_SR1(I2C2) &= ~I2C_SR1_AF;
			i2c_send_stop(I2C2);
			return -6;
		}
*/
	}
	if(timeout==0) { return -7; }

        /* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

        /* Cleaning I2C_SR1_ACK. */
        I2C_CR1(I2C1) &= ~I2C_CR1_ACK;

        /* Now the slave should begin to send us the first byte. Await BTF. */
	timeout=TIMEOUT_VALUE;
        while (!(I2C_SR1(I2C1) & I2C_SR1_BTF) && timeout!=0) timeout--;
	if(timeout==0) { return -8; }

        /* Send STOP condition. */
        i2c_send_stop(I2C1);
//        I2C_CR1(I2C2) |= I2C_CR1_STOP;

        /*
         * Yes they mean it: we have to generate the STOP condition before
         * saving the 1st byte.
         */

	*value = I2C_DR(I2C1);
	*(value+1) = I2C_DR(I2C1);

        /* Original state. */
        I2C_CR1(I2C1) &= ~I2C_CR1_POS;

        return 1;

}

// 1/(100kHz)
#define I2C_DELAY for(i=0; i<2400; i++) __asm__("nop")
void i2c1_recover() {
	u32 timeout;
	volatile long int i;

	printf("i2c1_recover();\r\n");

	// SDA HIGH
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_I2C1_SDA);

	// SCL HIGH
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_I2C1_SCL);

	timeout=0;
	while (!gpio_get(GPIOB, GPIO_I2C1_SDA)) { // if SDA == LOW continue with clocks

		if(timeout++ >= 1000) {
			printf("i2c1_recover() timeout..\r\n");
			break;
		}
		// SCL LOW
		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_I2C1_SCL);
		gpio_clear(GPIOB, GPIO_I2C1_SCL);

		I2C_DELAY;

		// SCL HIGH
		gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_I2C1_SCL);

		I2C_DELAY;
	}

	// Send a STOP

	// SDA LOW
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_I2C1_SDA);
	gpio_clear(GPIOB, GPIO_I2C1_SDA);

	I2C_DELAY;

	// SDA HIGH
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_I2C1_SDA);

	// I2C ready for normal operation
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL | GPIO_I2C1_SDA);

}

