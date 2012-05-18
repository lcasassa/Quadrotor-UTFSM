#ifndef I2C1_H
#define I2C1_H

void i2c1_setup();
int i2c1_read(char sla, char reg_addr, char * value, char len);
int i2c1_write(char sla, char reg_addr, char value);
void i2c1_recover();

#endif
