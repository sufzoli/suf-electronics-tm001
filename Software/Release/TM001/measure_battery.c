/*
 * measure_battery.c
 *
 *  Created on: 2013.05.15.
 *      Author: zoli
 */

/** Reads the MSP430 supply voltage using the Analog to Digital Converter (ADC).
On ez430 boards, this is approx. 3600mV
@return Vcc supply voltage, in millivolts
*/
#include <msp430.h>
#include "measure_battery.h"

unsigned int ADC_value;
unsigned char ADC_busy;

void ADC_init()
{
	ADC10CTL0 = SREF_1 + REF2_5V + ADC10ON + ADC10SHT_3 + ADC10IE;  // use internal ref, turn on 2.5V ref, set samp time = 64 cycles, reference kept off
	ADC10CTL1 = INCH_11;
	ADC_value = 0xFFFF;	// invalid data until the first conversion
	ADC_busy = 0;
}

void ADC_refon()
{
	ADC10CTL0 |= REFON;
}

void ADC_start_conversion()
{
	ADC_busy = 1;
	ADC10CTL0 |= ENC + ADC10SC;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	ADC_value = ADC10MEM;
	ADC10CTL0 &= ~REFON;	// Switch off reference voltage
	ADC_busy = 0;
}



