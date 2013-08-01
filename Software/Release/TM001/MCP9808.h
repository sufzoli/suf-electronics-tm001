/*
 * MCP9808.h
 *
 *  Created on: 2013.05.09.
 *      Author: zoli
 */

#ifndef MCP9808_H_
#define MCP9808_H_

#define MCP9808_ADDR		0x18
#define MCP9808_REG_CONFIG	1
#define MCP9808_REG_TEMP	5


unsigned int MCP9808_read_temp();
void MCP9808_LPM(unsigned char shutdown);

#endif /* MCP9808_H_ */
