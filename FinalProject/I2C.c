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
/* Cac chan I2C cua MSP430G2553 */                             
//                             
//             -----------------           
//         /|\|              XIN|-    
//          | |                 |      
//          --|RST          XOUT|-     
//            |                 |         
//            |                 |         
//            |                 |        
//            |         SCL/P1.6|-------> 
//            |         SDA/P1.7|-------> 
//            |                 | 
//            |                 |  
//            |                 | 
//            ------------------- 
// 
//Note: Rut Jumper noi loi LED noi voi chan P1.6 ra ngoai! 
//Ko can dien tro keo len. Vi MSP ben trong da co san, chung ta chi can  
//khai bao su dung cac dien tro keo len trong chuong trinh! 
  
  
  
#include "msp430G2553.h" 
  
void i2c_init(unsigned char sla_add) 
{  
     P1SEL |= BIT6 + BIT7;                      // Assign I2C pins to USCI_B0 
     P1SEL2|= BIT6 + BIT7;                      // Assign I2C pins to USCI_B0 
     UCB0CTL1 |= UCSWRST;                       // Enable SW reset 
     UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;          // I2C Master, synchronous mode 
     UCB0CTL1 = UCSSEL_2+UCSWRST;               // Use SMCLK, keep SW reset 
     UCB0BR0 = 12;                              // fSCL = SMCLK/12 = ~100kHz 
     UCB0BR1 = 0; 
     UCB0I2CSA = sla_add;                       // Set slave address 
     UCB0CTL1 &= ~UCSWRST;                      // Clear SW reset, resume operation  
}   
void i2c_read(unsigned char AddRes,unsigned char numbyte,unsigned char *poirev) 
{       
     //unsigned char *poirev;
     while (UCB0CTL1 & UCTXSTP);                // Loop until I2C STT is sent 
     UCB0CTL1 |= UCTR + UCTXSTT;                // I2C TX, start condition 
       
     while (!(IFG2&UCB0TXIFG));                
     UCB0TXBUF = AddRes;                        // Address start 
       
     while (!(IFG2&UCB0TXIFG));   
       
     UCB0CTL1 &= ~UCTR;                         // I2C RX  
     UCB0CTL1 |= UCTXSTT;                       // I2C start condition     
     IFG2 &= ~UCB0TXIFG;                        // Clear USCI_B0 TX int flag  
       
     while (UCB0CTL1 & UCTXSTT);                // Loop until I2C STT is sent 
     while (!(IFG2&UCB0RXIFG));  
     unsigned char i; 
     for( i=0;i<=numbyte;i++) 
      { 
        *(poirev+i-1)= UCB0RXBUF;
        //datarev[i]= *(poirev+i); 
         while (!(IFG2&UCB0RXIFG)); 
          
      } 
      
     UCB0CTL1 |= UCTXSTP;                       // I2C stop condition after 1st TX 
        
} 
void i2c_write(unsigned char AddRes,unsigned char numbyte, unsigned char *poisend) 
{ 
     while (UCB0CTL1 & UCTXSTP);                // Loop until I2C STT is sent 
     UCB0CTL1 |= UCTR + UCTXSTT;                // I2C TX, start condition 
    
     while (!(IFG2&UCB0TXIFG));  
     UCB0TXBUF = AddRes; 
       
     while (!(IFG2&UCB0TXIFG));  
     unsigned char i; 
     for( i=0;i<numbyte;i++) 
      { 
         UCB0TXBUF= *(poisend+i) ; 
         while (!(IFG2&UCB0TXIFG));  
      } 
     UCB0CTL1 |= UCTXSTP;                       // I2C stop condition after 1st TX 
     IFG2 &= ~UCB0TXIFG;                        // Clear USCI_B0 TX int flag  
}
