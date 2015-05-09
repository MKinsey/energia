// the squatch temperature array‎
//-----------Ooooo--
//-----------(----)---
//------------)--/----
//------------(_/-
//Used code examples from the GFDS18B20 Library. Author is listed below:
// Grant Forest 29 Jan 2013.

#include <GFDS18B20.h>  // One Wire Library for ds18b20 sensor
#include <RTC_B.h>      // Library for interacting with the on-board RTC_B


#define LED RED_LED
#define OWPIN  P1_3  // p1.3
#define MAXOW 100  //Max number of OW's used

// Global Variables
byte ROMarray[MAXOW][8];
byte ROMtype[MAXOW];     // 28 for temp', 12 for switch etc.
byte ROMtemp[MAXOW];
int32_t result[MAXOW+5];
byte data[12];
byte i;
byte addr[8];
uint8_t ROMmax=0;
uint8_t ROMcount=0;
boolean foundOW =false;
DS18B20 ds(OWPIN);
 // Debug mode will display information via the serial port. 
 //  An on-board LED (red) will indicate it is in debug mode
boolean debug_mode = true; 

void setup(void) {
  //set all pins low
  //P1_0 - _7
  pushLow(P1_0);
  //P2-0 - _6
  //P3-0 - _3_6
  if(debug_mode) {
    debug_init();
    debug_findOW();
    displayOW();
  }
  else {
    findOW();
  }
 
  rtc.begin();
  rtc.setTimeStringFormat(true, true, false, true, false);  // (use_24hr=True, use_shortwords, day_before_month, short_date_notation, include_seconds)
}

void loop(void) {
  char current_time[64];   // Temporary char[] buffer to hold time string
  // Get data from one wire bus
  tempCMD();
  for (i=1; i<ROMmax+1;i++){
      if (ROMtype[i]==0x28) {
         readOW(i); 
         saveTemperature(i);
       }
  }  
  for (i=1;i<ROMmax+1;i++){
    if (ROMtype[i]==0x28) {
        foundOW=true;
         Serial.print("Sensor ");
         Serial.print(i);
         Serial.print(":");
         prt2((result[i]*625)/100);
         Serial.print("C ");
        
         Serial.print("\t Time: ");
         rtc.getTimeString(current_time);
         Serial.println(current_time);
     }  
 } 
 if (foundOW) Serial.println(); 
 // Get timestamp from RTC (lib at 430h)
 // Write to SD card (spoof)
 // Sleep in LPM3 for 600
 sleepSeconds(60); 

}

void tempCMD(void){      //Send a global temperature convert command
  ds.reset();
  ds.write_byte(0xcc);  // was ds.select(work); so request all OW's to do next command
  ds.write_byte(0x44);  // start conversion, with parasite power on at the end
  delay(1000);
}
void saveTemperature(uint8_t ROMno){
  result[ROMno]=(int32_t)((data[1] <<8) | data [0]);
}

// Sets a pin to output and low in order to conserve power
void pushLow(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
void readOW(uint8_t ROMno)
{
   uint8_t i;
   ds.reset();
   ds.select(ROMarray[ROMno]);
   ds.write_byte(0xBE);         // Read Scratchpad
   for ( i = 0; i < 9; i++) {   // need 9 bytes
      data[i] = ds.read_byte();
#if TEST
      if (data[i]<16) Serial.print("0");
      Serial.print(data[i], HEX);
      Serial.print(" ");
#endif   
  }
}
// findOW without using the Serial port
void findOW(void)
{
 byte addr[8]; 
 uint8_t i; 
 ROMmax=0;  ///////////////////////////////////////////////////////
 while (true){  //get all the OW addresses on the buss
   i= ds.search(addr);
   if ( i<10) {
      ds.reset_search();
      delay(500);
      return;
    }
    for( i = 0; i < 8; i++) {
       if (i==0)  ROMtype[ROMmax+1]=addr[i];  // store the device type       
      ROMarray[ROMmax+1][i]=addr[i];     
    }
    ROMmax++;  
 } 
}

// findOW for debug mode (uses serial port)
void debug_findOW(void)
{
 byte addr[8]; 
 uint8_t i; 
 ROMmax=0;  ///////////////////////////////////////////////////////
 while (true){  //get all the OW addresses on the buss
   i= ds.search(addr);
   if ( i<10) {
      Serial.print("ret=("); 
      Serial.print(i);
      Serial.print(") No more addresses.\n");
      ds.reset_search();
      delay(500);
      return;
    }
   Serial.print("R=");
    for( i = 0; i < 8; i++) {
       if (i==0)  ROMtype[ROMmax+1]=addr[i];  // store the device type
         
      ROMarray[ROMmax+1][i]=addr[i];     

      if (addr[i]<16) Serial.print("0"); 
      Serial.print(addr[i], HEX);
      Serial.print(" ");
    }
    ROMmax++;
  Serial.print ("\t(OW");
    Serial.print (ROMmax,HEX);
    Serial.print (") Type="); 
    Serial.println (ROMtype[ROMmax],HEX);
  
    
 } 
}


void displayOW(void)
{
  uint8_t i;
  Serial.println ("From array");
  for (ROMcount=1; ROMcount<ROMmax+1; ROMcount++) {   
  ds.reset();
    for( i = 0; i < 8; i++) {
      if (ROMarray[ROMcount][i]<16) Serial.print("0");
      Serial.print( ROMarray[ROMcount][i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
}

void prt2(int x){
  Serial.print(x/100);
  Serial.print(".");
  Serial.print(x%100);
}

void debug_init(){
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); 
  Serial.begin(9600);
  delay(500);
  Serial.print("Squatch Temp Array\n DEBUG MODE\n");
}
