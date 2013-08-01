/*
 * measure_battery.h
 *
 *  Created on: 2013.05.16.
 *      Author: zoli
 */

#ifndef MEASURE_BATTERY_H_
#define MEASURE_BATTERY_H_

extern unsigned char ADC_busy;
extern unsigned int ADC_value;

void ADC_init();
void ADC_refon();
void ADC_start_conversion();


#endif /* MEASURE_BATTERY_H_ */
