#include <msp430.h>
//#include "i2c_lcd.h" // i2c for lcd
#include "LCD16x2.h" //IO for lcd

// Date
int sec, min, hour, day , date , month, year;
//char  dayc,Sun,Mon,Tues,Wed,Thurs,Fri,Sat;
char* dayc[] = {"Sun", "Mon","Tues", "Wed", "Thurs", "Fri", "Sat"};
//Button


//Function
void displayDateTime();
void updateClock();

void InitLCD();
void InitButton();
void InitTimer(); // TIMER0_A0_VECTOR
void InitCountdown();
void InitAlrm();

void AlrmLCD();
int Alarm(); //return 1 if alarm -> speaker turn on
void  CountdownLCD();//  TIMER0_A1_VECTOR
int Countdown(); // if count == 0 -> speaker turn on
void AdjustTimerLCD();
void AdjustTimer();

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    InitLCD();

    InitTimer();
    InitButton();
    __bis_SR_register(GIE);
    while(1);
}

//timer here
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void) {
//    if (TA0CTL & TAIFG) {

        updateClock();
        displayDateTime();
//        TA0CTL &= ~TAIFG;
        TA0CCTL0 &= ~CCIFG; // clear interrupt flag
//    }
}
//#pragma vector = TIMER0_A1_VECTOR
//__interrupt void  ISR_TA0_CCR1 (void)
//{
//    TA0CCTL1 &= ~CCIFG;
//}
// Button here
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
//    volatile unsigned int i;
//    P1OUT ^= BIT0;   // Toggle LED connected to P1.0
//    for(i = 10000; i != 0; i--);
//    while(!(P1IN & BIT3));
//    for(i = 10000; i != 0; i--);
//    P1IFG &= ~BIT3;  // Clear the P1.3 interrupt flag
}
void displayDateTime() {
    LCD_Clear();
    LCD_Goto(0, 1);
    LCD_PrintString("Time: ");
    LCD_PrintUnDecimal(hour, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(min, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(sec, 2);

    LCD_Goto(0, 2);
    LCD_PrintString("Date: ");
    LCD_PrintUnDecimal(date, 2);
    LCD_Display('/');
    LCD_PrintUnDecimal(month, 2);
    LCD_Display('/');
    LCD_PrintUnDecimal(year, 4);

    delayms(400);
}
void updateClock() {
    sec++;
    volatile unsigned int i;
    if (sec >= 60) {
        sec = 0;
        min++;
        if (min >= 60) {
            min = 0;
            hour++;
            if (hour >= 24) {
                hour = 0;
            }
        }
    }
}
void InitLCD() {
    /**
        p1.1 -> RS
        P1.3 -> E
        //p1.2 -> RW == GND (Here is write data bit)
        p1.2 - p1.5 -> D4 - D7
        BLA = VCC
        BLK = GND
     */
    LCD_Init();
    LCD_Clear();
    LCD_Goto(1,1);
    LCD_PrintString("Final Project");
    LCD_Goto(1,2);
    LCD_PrintString("Digital Clock");
    delayms(5000);
}
void InitButton() {
    /*
     * P2.0 -> BUTTON LEFT
     * P2.1 -> BUTTON RIGHT
     * P2.2 -> BUTTON ENTER
     * P2.3 -> BUTTON EXIT
     * */
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    P2REN |= BIT0 + BIT1 + BIT2 + BIT3;
    P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;

    P2IES |= BIT0 + BIT1 + BIT2 + BIT3;
    P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    P2IE |= BIT0 + BIT1 + BIT2 + BIT3;
}
void InitTimer() {
    // is not yet equal 1s
    TA0CCR0 = 0xFFFF;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | ID_3 | MC_3 | TACLR;   // Select the clock source (SMCLK) and set the mode (Up/Down Mode)

}
void InitCountdown() {

//    TA0CCR1=0xFFFF;
//    TA0CCTL1=CCIE;
//    TACCTL1=TASSEL_2|ID_3|MC_2|TACLR;
    //    TA0CTL |= TACLR; // clear the counter
    //    TA0CTL |= TASSEL_2; // clock = SMCLK
    //    TA0CTL |= MC_3; // up mode
    //    TA0CTL |= ID_3;
    //    TA0CCR0 = 0xFFFF;
    //    TA0CCR1 = 1638;
    //    TA0CCTL0 &= ~CCIFG; // clear interrupt flag
    //    TA0CCTL0 |= CCIE; // enable TA0 CCR0 overflow IRQ
    //    TA0CCTL1 &= ~CCIFG; // clear interrupt flag
    //    TA0CCTL1 |= CCIE; // enable TA0 CCR1 overflow IRQ
}
int Alarm() {

}

