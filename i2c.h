#ifndef I2C_H
#define i2C_H

void i2c_setup();
int i2c_read(char sla, char reg_addr, char * value, char len);
int i2c_write(char sla, char reg_addr, char value);
void i2c_recover();

#endif
