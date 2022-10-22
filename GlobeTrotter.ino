/* Globe Trotter
 * A tangible and interactive picture viewer
 * an original idea by Caroline Buttet
 * www.carolinebuttet.ch
 * 
 * Libraries used:
 * CapitiveSense Library
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * 
 * The keyboard library from Arduino
 * 
 * and the Encoder library
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 */
#include <CapacitiveSensor.h>
#include <Encoder.h>

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

/*countries*/
//threshold for capacitive sensing. Modify to suit your needs. 
int threshold = 45;

//ENCODER
long oldPosition  = -999;

//ENCODER---------
//My rotary encoder is a chinese clone so I had to debounce it with capacitors, like explained here (https://idyl.io/2017/02/28/robodyn-24-steps-rotary-encoder-review/) 
// Connect a 0.1µF capacitor from ground to CLK   (for debouncing)
// Connect a 0.1µF capacitor from ground to DT    (for debouncing)
Encoder myEnc(2, 3);
///

RTC_DS1307 rtc;

//setting the capacitive sensors (1 sensor per country)
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);    //Madagascar      // 10M resistor between pins 4 & 5, pin 6 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);    //Namibia         // 10M resistor between pins 4 & 6, pin 7 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_7 = CapacitiveSensor(4,7);    //France          // 10M resistor between pins 4 & 7, pin 8 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);    //Ethiopia        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_9 = CapacitiveSensor(4,9);    //China           // 10M resistor between pins 4 & 9, pin 9 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_10 = CapacitiveSensor(4,10);  //Vietnam         // 10M resistor between pins 4 & 10, pin 10 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_11 = CapacitiveSensor(4,11);  //Philippines     // 10M resistor between pins 4 & 11, pin 11 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_12 = CapacitiveSensor(4,12);  //Japan           // 10M resistor between pins 4 & 12, pin 12 is sensor pin, add a wire and a pin to your desired country on your globe. 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup()                    
{
   //begin serial communication
   Serial.begin(9600);

   lcd.begin (16,2);  // initialize the lcd 
   lcd.backlight();//To Power ON the back light
   if (!rtc.begin()) 
   {
     lcd.print("Couldn't find RTC");
     while (1);
   }
   if (!rtc.isrunning()) 
   {
    lcd.print("RTC is NOT running!");
   }

   //auto update from computer time
   DateTime now = DateTime(F(__DATE__), F(__TIME__));
   rtc.adjust(now);
}

void loop()                    
{
  //update the encoder for the globe position.
  //updateEncoder();
  //update capacitive sensors to find out if a country has been touched. 
  updateCapacitiveSensors();
  //display time on I2C LCD
  display();
}


void updateEncoder(){
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    
    //I am only reading every 4 steps, otherwise the pictures will update too quickly.
    if(newPosition % 4 == 0){
       Serial.println(newPosition/4);
       if(newPosition > oldPosition){
        //GOING COUNTERCLOCKWISE

       }
       else{
        //GOIGN CLOCKWISE

       }
    }
    oldPosition = newPosition;
  } 
 }

void updateCapacitiveSensors(){
    long total1 =  cs_4_8.capacitiveSensor(30);   //Ethiopia
    long total2 =  cs_4_5.capacitiveSensor(30);   //Madagascar
    long total3 =  cs_4_6.capacitiveSensor(30);   //Namibia
    long total4 =  cs_4_7.capacitiveSensor(30);   //France
    
    long total5 =  cs_4_9.capacitiveSensor(30);   //China
    long total6 =  cs_4_10.capacitiveSensor(30);  //Vietnam
    long total7 =  cs_4_11.capacitiveSensor(30);  //Japan
    long total8 =  cs_4_12.capacitiveSensor(30);  //Philippines
    
    Serial.print("\t");                    // tab character for debug windown spacing
    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                  // print sensor output 3
    Serial.print("\t");
    Serial.print(total4);                // print sensor output 4
    
    Serial.print("\t");
    Serial.print(total5);                // print sensor output 5
    Serial.print("\t");
    Serial.print(total6);                // print sensor output 6
    Serial.print("\t");
    Serial.print(total7);                // print sensor output 7
    Serial.print("\t");
    Serial.println(total8);                // print sensor output 8

    DateTime now = rtc.now();
    int y = now.year();
    int mon = now.month();
    int d = now.day();
    int h = now.hour();
    int min = now.minute();
    int s = now .second();
    
    if(total1>threshold){
      if (h+1 <= 23){
        h++;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total2>threshold){
      if (h+2 <= 23){
        h += 2;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total3>threshold){
      if (h+3 <= 23){
        h += 3;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total4>threshold){
      if (h+4 <= 23){
        h += 4;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total5>threshold){
      if (h+5 <= 23){
        h += 5;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total6>threshold){
      if (h+6 <= 23){
        h += 6;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total7>threshold){
      if (h+7 <= 23){
        h += 7;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
    else if(total8>threshold){
      if (h+8 <= 23){
        h += 8;
      }else{
        h=0;
      }
      rtc.adjust(DateTime(y,mon,d,h,min,s));
    }
}

void display(){
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("TIME");
  lcd.print(" ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());
  lcd.print("  ");
}

