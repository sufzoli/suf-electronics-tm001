#include <msp430.h>
#include "pcf8576_lcd35.h"
#include "i2c_usi.h"
#include "MCP9808.h"
#include "systime.h"
#include "measure_battery.h"

/*
 * main.c
 * This is a beta project for MCP9808 based temperature meter. This runs on MSP430G2452 in the test environment. The final version is targeted to MSP430G2230 (SOIC-8)
 */

#define work_cycle	180		// time between two temperature measurement (~340ms * work_cycle)

int scheduler = 0;

void _init(void)
{
	ADC_init();			// Initialize ADC registers
    lcd35_init(1,1);	// Initialize I2C, I2C internal pull-up resistors plus PCF8576C LCD driver
    LCDFL |= LCD_P3;	// Switch on the decimal dot
    lcd35_send();		// Send the data to the LCD
}

void _worker()
{
	unsigned int temp;
	ADC_refon();	// Start ADC reference, the temp readout and the display will give enough time to stabilize
	temp = MCP9808_read_temp();	// Read actual temperature value

	// Multiple by 10 then shift right 4 bits
	temp = (temp << 2) + temp; // * 5
	temp >>= 3;	// *2, shift right 4 bits

    lcd35_write(temp);	// Write the temperature value to the LCD

    if(ADC_value < 550)		// The supply voltage is below ~2,7V
    {
    	LCDFL |= LCD_LOW_BAT;	// Switch on the Low Battery indicator
    }
    else
    {
		LCDFL &= ~LCD_LOW_BAT;	// switch of the Low Battery indicator
    }

    lcd35_send();	// send the data to the LCD
    ADC_start_conversion();	// start the ADC conversion cycle
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    set_dco(1);		// Set 1MHz clock

	// BIT 7-6 = 00 Select MCLK to DCOCLK
	// BIT 5-4 = 00 Set MCLK Clock Divider to /1
	// BIT   3 =  0 Select SMCLK to DCOCLK
	// BIT 2-1 = 00 Set SMCLK Clock Divider to /1
	// BIT   0 =  0 DCO resistor: internal (no meaning because using external is not possible)
	BCSCTL2 = 0;

	// Bit 5-4 = 11 Set ACLK Clock Divider to /8
	BCSCTL1 |= DIVA0 + DIVA1;

	// Bit 5-4 = 10 Select Low frequency clock to VLOCLK
	BCSCTL3 &= ~LFXT1S0;
	BCSCTL3 |= LFXT1S1;

	_init();	// initialize the peripheries

	__delay_cycles(800000);		// Startup delay - wait every peripheries

	// Setup Watchdog timer to provide the measurement time
    // WDTSSEL -  1 - Source ACLK (~12 kHz VLO)
	// WDTISx -  10 - clock/512 (23 - 24 Hz)
    // WDTTMSEL - 1 - Interval mode
    WDTCTL = WDTPW | WDTSSEL | WDTIS1 | WDTTMSEL;
    IE1 |= WDTIE; // Enable WDT interrupt

    _EINT();	// Enable interrupts

    // the main loop
	while(1)
	{
		// task sequence
		switch(scheduler)
		{
			case 0:
				// Wake Up MCP9808
				MCP9808_LPM(0);
				// lit conversion LCD segment
				LCDFL |= LCD_P1;
			    lcd35_send();
				break;
			case 1:
				_worker();
				// Shutdown MCP9808
				MCP9808_LPM(1);
				// switch of conversion LCD segment
			    LCDFL &= ~LCD_P1;
			    lcd35_send();
				break;
			case work_cycle:
				scheduler = -1;	// reset the sequence counter
				break;
		}
		scheduler++;
		if(ADC_busy)	// if ADC conversion is running we can only switch off the processor and not the SMCLK
		{
			LPM0;
		}
		else
		{
			LPM3;
		}
    }
}

// Watchdog timer interrupt
// Schedule the measurement operation
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	// reset WDT
	WDTCTL = WDTPW | WDTSSEL | WDTIS1 | WDTTMSEL | WDTCNTCL;
	// Clear interrupt
	IFG1 &= ~WDTIFG;
	// Exit low power mode
	LPM3_EXIT;
}



