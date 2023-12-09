/******************************************************************************
 *
 * www.payitforward.edu.vn
 *
 ******************************************************************************/

/******************************************************************************
 *
 * LAUNCH PAD CODING
 *
 ******************************************************************************/

/******************************************************************************
 *
 *    Module       : Basic_config.C
 *    Description  : This file describes some basic configuration functions
 *                   such as Initialization clock or delay
 *
 *  Tool           : CCS 5.1
 *  Chip           : MSP430G2xxx
 *  History        : 18-7-2012
 *  Version        : 1.6
 *
 *  Author         : Nguyen Tien Manh, CLB NCKH DDT (manhcly@ymail.com)
 *  Notes          :
 *      To apply these functions, you must include the header file Basic_config.h
 *      and add this code file to your project.
 *
******************************************************************************/

 /****************************************************************************
 * IMPORT
******************************************************************************/

#include <msp430.h>
#include "Basic_config.h"

 /****************************************************************************
* FUNCTIONS
******************************************************************************/

//*****************************************************************************
// Stop Watch-dog Timer
//*****************************************************************************
void Config_stop_WDT(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 	// Stop watchdog timer
	// WDTCTL WatchdogTimer+Register
		// WDTPW   = Watchdog timer + password.
			//Must be written as 05Ah, or a PUC (Power-Up Clear) is generated
		// WDTHOLD = Watchdog timer + hold.This bit stops the watchdog timer+
}

//*****************************************************************************
// Clocks Configurations
//*****************************************************************************
void Config_Clocks(void)
{
	if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF) // Check if constants cleared
	{
	  while(1);            			// If cal constants erased, trap CPU!!
	}

	BCSCTL1 = CALBC1_1MHZ; 			// Set DCO range & ACLK prescaler
	// BCSCTL1 - Basic Clock System Control Register 1
		// XT2OFF   = XT2 off. This bit turns off the XT2 oscillator
		// XTS      = LFXT1 mode select. 1: High-frequency mode
		// DIVAx    = Divider for ACLK
		// RSELx    = Range select
	DCOCTL = CALDCO_1MHZ;  			// Set DCO step + modulation
	// DCOCTL - DCO Control Register
		// DCOx     = DCO frequency select
		// MODx     = Modulator selection
	BCSCTL3 |= LFXT1S_2 + XCAP_3; // configure ACLK Source
	// BCSCTL3 - Basic Clock System Control Register 3
		// XT2Sx    = XT2 range select
		// LFXT1Sx  = Low-frequency clock select and LFXT1 range select
			// When XTS=0: 00 32768-Hz crystal on LFXT1
						//10 VLOCLK 12 KHz
			// When XTS=1: 01 1-to 3-MHz crystal or resonator
						//10 3-to 16-MHz crystal or resonator
		// XCAPx    = Oscillator capacitor selection. 11 ~12.5pF
		// XT2OF    = XT2 oscillator fault
		// LFXT1OF  = LFXT1 oscillator fault
	while(IFG1 & OFIFG)				// wait for OSCFault to clear
	{
	  IFG1 &= ~OFIFG;
	  __delay_cycles(100000);
	}
	// IFG1 - Interrupt Flag Register 1
		// OFIFG    = Oscillator fault interrupt flag

	//	_bis_SR_register(SCG1 + SCG0);	// Stop DCO

	BCSCTL2 |= SELM_0 + DIVM_0;		// select MCLK, SMCLK clock and prescaler
	// BCSCTL2 - Basic Clock System Control Register 2
		// SELMx   = Select MCLK. These bits select the MCLK source.
			//00 DCOCLK
			//01 DCOCLK
			//10 XT2CLK when XT2 present. LFXT1CLK or VLOCLK when XT2 not present
			//11 LFXT1CLK or VLOCLK
		// DIVMx   = Divider for MCLK
		// SELS    = Select SMCLK.This bit selects the SMCLK source.
			//0 DCOCLK
			//1 LFXT1CLK or VLOCLK when XT2 not present. XT2CLK when XT2 present.
		// DIVSx   = Divider for SMCLK
		// DCOR    = DCO resistor select
}

//*****************************************************************************
// Delay for t microseconds
// The delay is not exact when t is too small
//*****************************************************************************
void delay_us (int t)
{
	int i;
	for (i = 0; i<t; i++ )
		_delay_cycles(MCLK_F);
}

//*****************************************************************************
// Delay for t milliseconds
//*****************************************************************************
void delay_ms (int t)
{
	int i;
	for (i = 0; i<t; i++ )
		_delay_cycles(MCLK_F*1000);
}

/******************************************************************************
 * END OF Basic_config.C
*******************************************************************************/
