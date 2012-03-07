BINARY = main

OBJS = parser.o timer2.o pid.o flash_.o timer3.o kalman.o nvic_.o timer1.o exti_.o joystick.o motors.o timer4.o usart.o ringbuffer.o ITG3200.o ADXL345.o i2c.o

LDSCRIPT = stm32-h103.ld
LDFLAGS += -lm -g
#LDFLAGS += -Wl,--section-start=.flash=0x08000000 


include Makefile.include

gdb: main.elf
	openocd -f interface/olimex-arm-usb-ocd.cfg \
		    -f board/olimex_stm32_h103.cfg \
		    -c "init" -c "halt" -c "reset" -c "soft_reset_halt" \
		    -c init -c 'reset init'

#		    -c "stm32f1x mass_erase 0" \
#		    -c "flash write_image main.hex" \
#		    -c "reset" -c "sleep 10"

reset: 
	openocd -f interface/olimex-arm-usb-ocd.cfg -f board/olimex_stm32_h103.cfg -c "init" -c "reset" -c "shutdown"

