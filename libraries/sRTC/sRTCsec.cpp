/*
 sRTC.h - Library for adding simple RTC capabilities using MSP430 hardware
 By Yannick DEVOS (XV4Y) - 2012
 */
 
// include this library's description file
#include <sRTCsec.h>
#include <legacymsp430.h>
 
// Constructor /////////////////////////////////////////////////////////////////
 
RealTimeClockSec::RealTimeClockSec(void)
{
    RTC_sec = 0;
    RTC_min = 0;
    RTC_hr = 0;
 
    WDTCTL = WDTPW | WDTHOLD; // Kill watch-dog
 
    BCSCTL1 = DIVA_3;        // Clock = ACLK / 8
    BCSCTL3 |= (LFXT1S_0 | XCAP_3);        // Internal 12.5pF cap for 32KHz crystal
 
    TA1CCTL0 = CCIE;             //  CCR0 interupt activated
    TA1CCR0 = 4096-1;               // 4096 ticks of 32KHz XTal = 1 second => CCR0 counts N+1
    TA1CTL = TASSEL_1 | ID_3 | MC_1;  // Clock for TIMER 1 = ACLK, By 8 division, up front
 
};
 
// Methods /////////////////////////////////////////////////////////////////
 
void RealTimeClockSec::Inc_sec(void) {
    RTC_sec = RTC_sec + 1;            // Update seconds
    if (RTC_sec == 60) {
        RTC_sec=0;
        Inc_min();
    };
};
 
void RealTimeClockSec::Inc_min(void) {
    RTC_min = RTC_min + 1;            // Update minutes
    if (RTC_min == 60) {
        RTC_min=0;
        Inc_hr();
    };
};
 
void RealTimeClockSec::Inc_hr(void) {
    RTC_hr = RTC_hr + 1;              // Update hours
    if (RTC_hr == 24) {
        RTC_hr=0;
    };
};
 
void RealTimeClockSec::Set_Time(char hrs=0, char mins=0, char secs=0) {
    if ((hrs>=0 && hrs <24) && (mins>=0 && mins<60) && (secs>=0 && secs<60)) {
        RTC_hr = hrs;   // Set time to values given in parameters
        RTC_min = mins;
        RTC_sec = secs;
    };
};