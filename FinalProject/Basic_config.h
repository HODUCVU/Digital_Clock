/******************************************************************************
 *
 * www.payitforward.edu.vn
 *
 ******************************************************************************/

/******************************************************************************
 *
 * TI LAUNCH PAD CODING
 *
 ******************************************************************************/

/******************************************************************************
 *
 *    Module       : Basic_config.H
 *    Description  : This header file includes definitions and prototypes of
 *          basic configuration functions such as Initialization clock or delay
 *
 *  Tool           : CCS 5.1
 *  Chip           : MSP430G2xxx
 *  History        : 18-7-2012
 *  Version        : 1.6
 *
 *  Author         : Nguyen Tien Manh, CLB NCKH DDT (manhcly@ymail.com)
 *  Notes          :
 *      To apply these functions, you must include the this header file and add
 *      the Basic_config.c file to your project.
 *
 *      To use delay functions, pay attention to "#define MCLK"
******************************************************************************/

#ifndef BASIC_CONFIG_H_
#define BASIC_CONFIG_H_

 /****************************************************************************
* DEFINITIONS
******************************************************************************/

// These definitions help you to notice the system clocks
#define MCLK_F 1 // frequency of Master Clock in MHz
// You must change it to the right MCLK frequency that you configure through
// "Config_Clocks" function
// This definition affect delay functions
#define SMCLK_F 1000000 // frequency of Sub-System Master Clock in Hz
// You must change it to the right SMCLK frequency that you configure through
// "Config_Clocks" function
#define ACLK_F 12000 // frequency of Auxiliary Clock in Hz
// You must change it to the right ACLK frequency that you configure through
// "Config_Clocks" function

 /****************************************************************************
* FUNCTIONS 'S PROTOTYPES
******************************************************************************/

void Config_stop_WDT (void);
void Config_Clocks (void);
void delay_us (int t);
void delay_ms (int t);


#endif /* BASIC_CONFIG_H_ */
/******************************************************************************
 * END OF Basic_config.H
*******************************************************************************/
