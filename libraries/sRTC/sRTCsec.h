/*
 sRTC.h - Library for adding simple RTC capabilities using MSP430 hardware
 By Yannick DEVOS (XV4Y) - 2012
 
///////////////////////////////// How to use the lib
 
 * Create an instance of the object
 
 RealTimeClock myClock;
 
 * In order to have this RTC library working you should add this code to the end of your main program :
 
 interrupt(TIMER1_A0_VECTOR) Tic_Tac(void) {
    myClock.Inc_sec();            // Update secondes
 };
 
 * Enjoy the clock reading the RTC_sec, RTC_min, RTC_hr variables
 
 */
 
// ensure this library description is only included once
#ifndef RTCsec_h
#define RTCsec_h
 
// library interface description
class RealTimeClockSec
{
    // user-accessible "public" interface
public:
    RealTimeClockSec(void);
    void Set_Time(char hr, char mins, char secs);
    char RTC_sec; // This how you read the time, by reading the vars
    char RTC_min;
    char RTC_hr;
    void Inc_sec(void); // This method should be invoked by an Interrupt call (see top of the file comment)
    // A few private methods
private:
    void Inc_min(void);
    void Inc_hr(void);
};
 
#endif