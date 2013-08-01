/*
 * MCP9808.c
 *
 *  Created on: 2013.05.09.
 *      Author: zoli
 */

#include "i2c_usi.h"
#include "MCP9808.h"

unsigned int MCP9808_read_temp()
{
	unsigned char buff[2];
	unsigned int retvalue;
    i2c_reg_read(MCP9808_ADDR, MCP9808_REG_TEMP, 2, &buff[0]);
    retvalue = buff[0] & 0x1F;
    retvalue <<= 8;
    retvalue += buff[1];
    return retvalue;
}

void MCP9808_LPM(unsigned char shutdown)
{
	unsigned char buff[3];
	buff[0] = MCP9808_REG_CONFIG;
	buff[1] = shutdown ? 1 : 0;
	buff[2] = 0;
	i2c_transmit(MCP9808_ADDR,3,&buff[0]);
}
