#ifndef I2c_H_
#define I2c_H_
/******************************************************************************
 *
 * WWW.PAYITFORWARD.EDU.VN
 *
 ******************************************************************************/

/******************************************************************************
 *
 * C7 TRAINING
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  Module         : I2C Interface
 *  Description    : Support some I2C functions for the communication with other chips
 *
 *  Tool           : CCS 5.1
 *  Chip           : MSP430G2553
 *  History        : 16-11-2012
 *  Version        : 1.2
 *
 *  Author         : THXN-PC
 *  Notes          : Remove the jumper connecting with PIN P1.6 
 *
 *
******************************************************************************/

void i2c_init(unsigned char sla_add);

void i2c_read(unsigned char AddRes,unsigned char numbyte,unsigned char *poirev);


void i2c_wirte(unsigned char AddRes,unsigned char numbyte, unsigned char *poisend);

#endif
