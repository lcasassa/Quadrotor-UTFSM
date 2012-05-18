#ifndef I2C2_H
#define I2C2_H

void i2c2_setup();
int i2c2_read(char sla, char reg_addr, char * value, char len);
int i2c2_write(char sla, char reg_addr, char value);
void i2c2_recover();

#endif
