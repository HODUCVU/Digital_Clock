#include <msp430.h>
#include "LCD16x2.h" //IO for lcd

// Date
unsigned int sec, min, hour, day , month, year, dayc;
char* date[] = {"Mon","Tues", "Wed", "Thurs", "Fri", "Sat", "Sun"};
//Button
void InitButton();
volatile int move;
volatile unsigned int screen_status;
volatile unsigned char config_status;
void moveEnter(unsigned int status);
void moveExit(unsigned int status);
void moveRight(unsigned int m, unsigned int status);
void moveLeft(unsigned int m, unsigned int status);
//Clock - screen_status = 0
void InitLCD();
void displayDateTime();
void updateClock();
void InitTimer(); // TIMER0_A0_VECTOR
//Menu - screen_status = 1
void InitMenu(int x, int y);
//Count down - screen_status = 13
int secx, minx, hourx, dayx, monthx, yearx, daycx; // temp
char countdown_status;
void InitCountdown(); // TIMER0_A1_VECTOR
void CountdownLCD();//  TIMER0_A1_VECTOR
void Countdown(); // if count == 0 -> speaker turn on
//Alarm - screen_status = 11
unsigned long alarm; // == sec + minute *10 + hour *100 + day * 1000
void InitAlrm();
void AlrmLCD(int x, int y);
void AlrmSet();
void Alarm(); //return 1 if alarm -> speaker turn on
//Set time - screen_status = 12
void SetTime();
void ConfigHour(int x, int y);
void ConfigDay(int x, int y);
void ConfigDate();

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    InitLCD();
    InitTimer();
    InitButton();
    InitAlrm();

    __bis_SR_register(LPM0_bits+GIE);
    while(1);
}

//timer here
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_TA0_CCR0(void) {
       updateClock();
       if(screen_status == 0)
           displayDateTime();
       if(alarm != 0)
       {
          Alarm();
       }
       TA0CCTL0 &= ~CCIFG;
}
//Count Down
#pragma vector = TIMER0_A1_VECTOR
__interrupt void  ISR_TA0_CCR1 (void)
{
    Countdown();
    TA0CCTL1 &= ~CCIFG;
}
// Button here
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    volatile unsigned int i;
    switch(P2IFG & (BIT0 | BIT1 | BIT2 | BIT3)) {
    case BIT0: //ENTER
        moveExit(screen_status);
        for(i = 1000; i != 0; i--);
        while(!(P2IN & BIT0));
        for(i = 1000; i != 0; i--);
        P2IFG &= ~BIT0;
        break;
    case BIT1: //EXIT
        moveEnter(screen_status);
        for(i = 1000; i != 0; i--);
        while(!(P2IN & BIT1));
        for(i = 1000; i != 0; i--);
        P2IFG &= ~BIT1;
        break;
    case BIT2: // RIGHT (TOP)
        moveRight(move, screen_status);
        for(i = 1000; i != 0; i--);
        while(!(P2IN & BIT2));
        for(i = 1000; i != 0; i--);
        P2IFG &= ~BIT2;
        break;
    case BIT3: // LEFT (BOTTOM)
        moveLeft(move, screen_status);
        for(i = 1000; i != 0; i--);
        while(!(P2IN & BIT3));
        for(i = 1000; i != 0; i--);
        P2IFG &= ~BIT3;
        break;
    default:
        P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    }
}
void displayDateTime() {
    screen_status = 0;
    move = 0;
    LCD_Clear();
    LCD_Goto(0, 1);
    LCD_PrintString("Thu");
    LCD_Goto(4, 1);
    LCD_PrintUnDecimal(hour, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(min, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(sec, 2);

    LCD_Goto(14, 1);
    LCD_PrintString("VN");

    LCD_Goto(0, 2);
    LCD_PrintString(date[dayc]);
    LCD_Goto(4, 2);
    LCD_PrintUnDecimal(day, 2);
    LCD_Display('/');
    LCD_PrintUnDecimal(month, 2);
    LCD_Display('/');
    LCD_PrintUnDecimal(year, 4);
    delayms(400);
}
void updateClock() {
    sec++;
    if (sec >= 60) {
        sec = 0;
        min++;
        if (min >= 60) {
            min = 0;
            hour++;
            if (hour >= 24) {
                hour = 0;
                day++;
                dayc++;
                if(dayc == 7)
                    dayc = 0;
               if ((month == 4 || month == 6 || month == 9 || month == 11) && day == 31)
               {
                   day = 1;
                   month++;
               }
               else if (month == 2 && day == 29) // Leap year check
               {
                   if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                   {
                       // Leap year
                       if (day == 30)
                       {
                           day = 1;
                           month++;
                       }
                   }
                   else
                   {
                       day = 1;
                       month++;
                   }
               }
               else if (month != 2 && day == 32)
               {
                   day = 1;
                   month++;
               }

               if (month == 13)
               {
                   month = 1;
                   year++;
               }
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
    screen_status = 0;
    year = 2023;
    month = 12;
    day = 11;
    LCD_Init();
    LCD_Clear();
    LCD_Goto(1,1);
    LCD_PrintString("Final Project");
    LCD_Goto(1,2);
    LCD_PrintString("Digital Clock");
    delayms(2000);
}
void InitButton() {
    /*
     * P2.0 -> BUTTON EXIT
     * P2.1 -> BUTTON ENTER
     * P2.2 -> BUTTON RIGHT (TOP)
     * P2.3 -> BUTTON LEFT (BOTTOM)
     * */
    P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    P2REN |= BIT0 + BIT1 + BIT2 + BIT3;
    P2OUT |= BIT0 + BIT1 + BIT2 + BIT3;

    P2IES |= BIT0 + BIT1 + BIT2 + BIT3;
    P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    P2IE |= BIT0 + BIT1 + BIT2 + BIT3;
}
void InitTimer() {
    TA0CCR0 = 0xFFFF;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | ID_3 | MC_3 | TACLR;
}
void InitMenu(int x, int y) {
    screen_status = 1;
    countdown_status = 0;
    LCD_Clear();
    LCD_Goto(0,1);
    LCD_PrintString("Alarm");
    LCD_Goto(0,2);
    LCD_PrintString("CountDown");
    LCD_Goto(8,1);
    LCD_PrintString("SetTime");
    LCD_Goto(10,2);
    LCD_PrintString("RESET");
    showPointer(x,y);
    delayms(500);
}
void moveEnter(unsigned int status) {
    switch(status) {
    case 0: // Clock
        InitMenu(5,1);
        break;
    case 1: // Menu
        switch(move) {
        case 0: // Alarm
            //move to Alarm -> screen_status = 11
            hourx = 0; minx = 0;
            AlrmLCD(8,2);
            config_status = 0;
            break;
        case 1: // Set time -> screen_status = 121
            // move to Set time
            hourx = hour; minx = min; secx = sec;
            ConfigHour(6,2);
            config_status = 0;
            break;
        case 2: // Count Down -> screen_status = 13
            hourx = 0; minx = 0; secx = 0;
            ConfigHour(6,2);
            config_status = 0;
            countdown_status = 1;
            break;
        case 3: // Reset
            break;
        default:
        }
        break;
    case 11: // Alarm -> displayDateTime()
        AlrmSet();
        break;
    case 121: // Set Hour - > Set Day
        switch(config_status) {
        case 0: // hour -> min
            ConfigHour(9,2);
            config_status = 1;
            break;
        case 1: // min -> sec
            ConfigHour(12,2);
            config_status = 2;
            break;
        case 2: // sec -> move
            config_status = 0;
            if(countdown_status == 1)
            {
                InitCountdown();
            } else
            {
                dayx = day; monthx = month; yearx = year;
                ConfigDay(8,2);
            }
            break;
        }
        break;
    case 122: // Set Day -> Set Date
        switch(config_status) {
        case 0: // year -> month
            ConfigDay(11,2);
            config_status = 1;
            break;
        case 1: // month -> day
            ConfigDay(14,2);
            config_status = 2;
            break;
        case 2: // day -> move
            config_status = 0;
            daycx = dayc;
            ConfigDate();
            break;
        }
        break;
    case 123: // Set Date -> displayDateTime()
        // Save time
        hour = hourx; min = minx; sec = secx; day = dayx; month = monthx; year = yearx; dayc = daycx;
        displayDateTime();
        break;
    case 13: // Count down
        countdown_status = 0;
        P2OUT |= BIT4;
        displayDateTime();
        TA0CCTL1 = ~CCIE; // Stop timer0_A1
        break;
    default:
        // Reset
    }
}
void moveExit(unsigned int status) {
    switch(status) {
    case 0: // Clock
        alarm = 0;
        P2OUT |= BIT4;
        break;
    case 1: // Menu
        displayDateTime();
        break;
    case 13: // Count down -> Menu()
        InitMenu(5,1);
        move = 0;
        P2OUT |= BIT4;
        countdown_status = 0;
        TA0CCTL1 = ~CCIE; // Stop timer0_A1
        break;
    default:
        // Reset
        InitMenu(5,1);
        move = 0;
    }
}
void moveRight(unsigned int m, unsigned int status){
    switch(status) {
        case 0: // Clock
            // not move
            break;
        case 1: // Menu
            move++;
            if(move == 4)
               move = 0;
            switch(move) {
            case 0:
                InitMenu(5,1);
                break;
            case 1:
                InitMenu(15,1);
                break;
            case 2:
                InitMenu(9,2);
                break;
            case 3:
                InitMenu(15,2);
                break;
            }
            break;
        case 11: // Alarm
            switch(config_status) {
             case 0:
                 hourx++;
                 if(hourx == 24)
                     hourx = 0;
                 break;
             case 1:
                 minx++;
                 if(minx == 60)
                     minx = 0;
                 break;
             }
             SetTime();
            break;
         case 121: // Set Hour
             switch(config_status) {
             case 0:
                 hourx++;
                 if(hourx == 24)
                     hourx = 0;
                 break;
             case 1:
                 minx++;
                 if(minx == 60)
                     minx = 0;
                 break;
             case 2:
                 secx++;
                 if(secx == 60)
                     secx = 0;
                 break;
             }
             SetTime();
             break;
         case 122: // Set Day
             switch(config_status) {
             case 2:
                 dayx++;
                 if ((monthx == 4 || monthx == 6 || monthx == 9 || monthx == 11) && dayx >= 31) {
                       dayx = 1;
                 }
                 else if (monthx == 2 && dayx == 29) // Leap year check
                 {
                   if ((yearx % 4 == 0 && yearx % 100 != 0) || (yearx % 400 == 0))
                   {
                       // Leap year
                       if (dayx == 30)
                       {
                           dayx = 1;
                       }
                   }
                   else
                   {
                       dayx = 1;
                   }
                 }
                 else if (monthx != 2 && dayx >= 32)
                 {
                   dayx = 1;
                 }
                 break;
             case 1:
                 monthx++;
                 if(monthx == 13)
                     monthx = 1;
                 break;
             case 0:
                 yearx++;
                 break;
             }
             SetTime();
             break;
         case 123: // Set Date
             daycx++;
             if(daycx >= 7)
                 daycx = 0;
             ConfigDate();
             break;
         case 13: // count down
             break;
         default:
       }
}
void moveLeft(unsigned int m, unsigned int status) {
    switch(status) {
        case 0: // Clock
            // not move
            break;
        case 1: // Menu
            move--;
            if(move == -1)
               move = 3;
            switch(move) {
            case 0:
                InitMenu(5,1);
                break;
            case 1:
                InitMenu(15,1);
                break;
            case 2:
                InitMenu(9,2);
                break;
            case 3:
                InitMenu(15,2);
                break;
            }
            break;
        case 11: // Alarm
            switch(config_status) {
            case 0:
                hourx--;
                if(hourx == -1)
                    hourx = 23;
                break;
            case 1:
                minx--;
                if(minx == -1)
                    minx = 59;
                break;
            }
            SetTime();
            break;
        case 121: // Set Hour
            switch(config_status) {
            case 0:
                hourx--;
                if(hourx == -1)
                    hourx = 23;
                break;
            case 1:
                minx--;
                if(minx == -1)
                    minx = 59;
                break;
            case 2:
                secx--;
                if(secx == -1)
                    secx = 59;
                break;
            }
            SetTime();
            break;
        case 122: // Set Day
            switch(config_status) {
            case 2:
                dayx--;
                if ((monthx == 4 || monthx == 6 || monthx == 9 || monthx == 11) && dayx <= 0) {
                      dayx = 30;
                }
                else if (monthx == 2 && dayx <= 0) // Leap year check
                {
                  if ((yearx % 4 == 0 && yearx % 100 != 0) || (yearx % 400 == 0))
                  {
                      dayx = 29;
                  }
                  else
                  {
                      dayx = 28;
                  }
                }
                else if (monthx != 2 && dayx <= 0)
                {
                  dayx = 31;
                }
                break;
            case 1:
                monthx--;
                if(monthx == 0)
                    monthx = 12;
                break;
            case 0:
                yearx--;
                break;
            }
            SetTime();
            break;
        case 123: // Set Date
            daycx--;
            if(daycx <= -1)
                daycx = 6;
            ConfigDate();
            break;
         case 13: // count down
             break;
        default:
       }
}

// Count Down
void InitCountdown() {
    screen_status = 13;
    TA0CCR1=0xFFFF;
    TA0CCTL1=CCIE;
    CountdownLCD();
}
void  CountdownLCD() {
//    ConfigHour -> CountdownLCD;
//    run down timer
    LCD_Clear();
    LCD_Goto(3, 1);
    LCD_PrintString("COUNT DOWN");
    LCD_Goto(4, 2);
    LCD_PrintUnDecimal(hourx, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(minx, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(secx, 2);
}
void Countdown() { // if count == 0 -> speaker turn on
    if(countdown_status == 1) {
        if(hourx == 0 && minx == 0 && secx == 0)
        {
            P2OUT ^= BIT4;
        }
        else {
            secx--;
            if(secx == -1) {
                minx--; secx = 59;
                if(minx == -1) {
                    hourx--; min = 59;
                }
            }
        }
    }
    CountdownLCD();
}
//Set time
void ConfigHour(int x, int y) {
    screen_status = 121;
    LCD_Clear();
    LCD_Goto(6, 1);
    LCD_PrintString("TIME");
    LCD_Goto(4, 2);
    LCD_PrintUnDecimal(hourx, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(minx, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(secx, 2);
    showPointer(x,y);
}
void ConfigDay(int x, int y) {
    screen_status = 122;
    LCD_Clear();
    LCD_Goto(7, 1);
    LCD_PrintString("DAY");
    LCD_Goto(4, 2);
    LCD_PrintUnDecimal(yearx, 4);
    LCD_Display('/');
    LCD_PrintUnDecimal(monthx, 2);
    LCD_Display('/');
    LCD_PrintUnDecimal(dayx, 2);
    showPointer(x,y);
}
void ConfigDate() {
    screen_status = 123;
    LCD_Clear();
    LCD_Goto(6, 1);
    LCD_PrintString("DATE");
    LCD_Goto(4, 2);
    LCD_PrintString("- ");
    LCD_PrintString(date[daycx]);
    LCD_PrintString(" +");
}
void SetTime() {
    switch(screen_status) {
    case 11:
        switch(config_status) {
        case 0:
            AlrmLCD(8,2);
            break;
        case 1:
            AlrmLCD(11,2);
            break;
        }
        break;
    case 121:
        switch(config_status) {
        case 0:
            ConfigHour(6,2);
            break;
        case 1:
            ConfigHour(9,2);
            break;
        case 2:
            ConfigHour(12,2);
            break;
        }
        break;
    case 122:
       switch(config_status) {
       case 0:
           ConfigDay(8,2);
           break;
       case 1:
           ConfigDay(11,2);
           break;
       case 2:
           ConfigDay(14,2);
           break;
       }
       break;
    default:
    }
}
// Alarm
void InitAlrm() {
    alarm = 0;
    P2DIR |= BIT4;
    P2OUT |= BIT4; // when bit4 is low -> speaker turn on
}
void AlrmLCD(int x, int y) {
    screen_status = 11;
    LCD_Clear();
    LCD_Goto(6, 1);
    LCD_PrintString("ALARM");
    LCD_Goto(6, 2);
    LCD_PrintUnDecimal(hourx, 2);
    LCD_Display(':');
    LCD_PrintUnDecimal(minx, 2);
    showPointer(x,y);
}
void AlrmSet() {
    switch(config_status) {
    case 0: // hour -> min
        AlrmLCD(11,2);
        config_status = 1;
        break;
    case 1: // min -> move
        config_status = 0;
        alarm = minx + hourx *10;
        displayDateTime();
        break;
    }
}
void Alarm(){ //return 1 if alarm -> speaker turn on
    unsigned long temp = min + hour *10;
    if(temp == alarm)
    {
        P2OUT ^= BIT4;
    } else 
    {
        P2OUT |= BIT4;
}
