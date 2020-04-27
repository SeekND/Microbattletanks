/*
   -- Micro Tank --

   by Carlos Costa (SeekNDFPV @ youtube)

   This source code of graphical user interface
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <Arduino.h>



////////////////////////////////// USER VALUES //////////////////////////////

#define REMOTEXY_WIFI_SSID "BATTLETANK1"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6789


byte middleright = 90;   // WHAT IS THE CENTER VALUE FOR THE RIGHT SERVO, adjust as necessary
byte middleleft = 90;    // WHAT IS THE CENTER VALUE FOR THE LEFT SERVO, adjust as necessary
byte updownrange = 20;   // WHAT IS THE MAX AND MIN RANGE OF THE SERVOS
uint32_t reloadingtime = 3000;    // RELOADING TIME IN MILLISECONDS
byte mindamage = 20;              // MINIMUM DAMAGE PER SHOT
byte maxdamage = 40;              // MAXIMUM DAMAGE PER SHOT
byte ammo = 200;                  // AMMO AVAILABLE - MAX IS 220
byte shutdowntimer = 65;           // TIME IT TAKES TO SHUTDOWN THE TANK IN SECONDS DIVIDED BY 2. eg: 65 = 6.5 SECONDS

int gettingshot[2] = {0xB47AF5B7, 0x4BB640BF}; // CODES FOR GETTING SHOT, IF YOU WANT TO ADD MORE CODES INCREASE THE NUMBER IN THE LEFT AND ENTER THE CODE IN THE RIGHT




////////////////////////////////////////////////////////////////// SERVO SETUP

#include <Servo.h>

int leftservopin = 13; // d7 pin in ESP8266
int rightservopin = 12; // d6 pin in ESP8266

Servo leftservo;
Servo rightservo;

////////////////////////////////////////////////////////////////// IR RECEIVER

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t kRecvPin = 14;  // d5 pin in ESP8266 for signal leg. The other two legs of the receiver connect to 5v and ground respectively.

IRrecv irrecv(kRecvPin);
decode_results results;

////////////////////////////////////////////////////////////////// IR TRANSMITTER

#include <IRsend.h>
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).  The ground leg of the ir led will connect to GROUND
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.


// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                        600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                        600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                        650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                        650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                        650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                        650, 1650, 650, 1650, 650, 1650, 600
                       };


////////////////////////////////////////////////////////////////// REMOTEXY SETTINGS


// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
{ 255, 6, 0, 31, 0, 58, 0, 10, 28, 0,
  1, 0, 43, 47, 16, 16, 38, 37, 70, 73,
  82, 69, 0, 67, 4, 21, 1, 61, 6, 31,
  26, 31, 1, 1, 42, 7, 17, 7, 115, 10,
  82, 69, 80, 65, 73, 82, 0, 5, 32, 52,
  8, 50, 50, 2, 26, 31, 5, 32, -1, 8,
  51, 51, 2, 26, 31
};

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  uint8_t button_1; // =1 if button pressed, else =0
  uint8_t button_2; // =1 if button pressed, else =0
  int8_t joystick_1_x; // =-100..100 x-coordinate joystick position
  int8_t joystick_1_y; // =-100..100 y-coordinate joystick position
  int8_t joystick_2_x; // =-100..100 x-coordinate joystick position
  int8_t joystick_2_y; // =-100..100 y-coordinate joystick position

  // output variables
  char text_1[31];  // string UTF8 end zero

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


////////////////////////////////////////////////////////////////// DEBUG

bool debugging = false;


////////////////////////////////////////////////////////////////// GLOBAL SETTINGS

byte middlerightreset = middleright;
byte middleleftreset = middleleft;


uint32_t reload;
byte health = 100;
bool tankstarted = true;
byte maxrangeright = middleright + updownrange;
byte minrangeright = middleright - updownrange;

byte maxrangeleft = middleleft + updownrange;
byte minrangeleft = middleleft - updownrange;
byte steering1;
byte steering2;
uint32_t takeabreath = 600000;

int shutdowntank = 0;
int shots = 0;



void setup()
{
  RemoteXY_Init ();
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);


  leftservo.attach(leftservopin, 1000, 2000);
  rightservo.attach(rightservopin, 1000, 2000);
  tankstarted = true;
  takeabreath = millis();


  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266

}


void loop()
{
  RemoteXY_Handler ();

  if (debugging == true) {

    int val1 = steering2;
    int val2 = steering1;
    sprintf (RemoteXY.text_1, "Left: %u  / Right: %d", val1, val2);


    if (irrecv.decode(&results)) {
      int valX = ((uint32_t) (results.value), HEX);
      sprintf (RemoteXY.text_1, "SHOT: %u", valX);
      delay(200);
    }
  }
  else
  {
    if ( health == 0 )
    {
      sprintf (RemoteXY.text_1, "DESTROYED. Hold repair.");
    }
    else if ( reload + reloadingtime > millis() )
      sprintf (RemoteXY.text_1, "RELOADING");
    else if (!tankstarted)
    {
      int val1 = health;
      sprintf (RemoteXY.text_1, "Health: %u SHUTDOWN", val1);
    }
    else if (RemoteXY.button_2 != 0)
    {
      if ( middleright < middlerightreset )
      {
        int val1 = middlerightreset - middleright;
        sprintf (RemoteXY.text_1, "Repairing right: %u dmg", val1);
      }
      else if ( middleright > middlerightreset)
      {
        int val1 = middleright - middlerightreset;
        sprintf (RemoteXY.text_1, "Repairing right: %u pts dmg", val1);
      }
      else if ( middleleft < middleleftreset)
      {
        int val1 = middlerightreset - middleleft;
        sprintf (RemoteXY.text_1, "Repairing left: %u pts dmg", val1);
      }
      else if ( middleleft > middleleftreset)
      {
        int val1 = middleleft - middlerightreset;
        sprintf (RemoteXY.text_1, "Repairing left: %u pts dmg", val1);
      }
      else
      {
        sprintf (RemoteXY.text_1, "Repaired");
      }

    }
    else
    {
      int val1 = health;
      int val2 = shots;
      sprintf (RemoteXY.text_1, "Health: %u Shots fired: %d", val1, val2);
    }
  }



  if (RemoteXY.button_2 != 0 ) {    // This is rhe repair button actions

    if (!tankstarted) {              // Tank is shutdown, servos will not work

      leftservo.attach(leftservopin, 1000, 2000); // d7
      rightservo.attach(rightservopin, 1000, 2000); // d8
      tankstarted = true;

      takeabreath = millis();

      if (health == 0)
      {
        middleright = middlerightreset;
        maxrangeright = middlerightreset + updownrange;
        minrangeright = middlerightreset - updownrange;


        middleleft = middleleftreset;

        maxrangeleft = middleleftreset + updownrange;
        minrangeleft = middleleftreset - updownrange;
        health = 100;
        shots = 0;
      }
    }
    else if (tankstarted && health > 0 && shutdowntank < 2 ) {            // This activates the repair of the tank
      if (steering1 > 10 && steering1 < 170)
      {
        if ( middleright < middlerightreset )
        {
          middleright++;
          maxrangeright = middleright + updownrange;
          minrangeright = middleright - updownrange;
        }
        else if ( middleright > middlerightreset)
        {
          middleright--;
          maxrangeright = middleright + updownrange;
          minrangeright = middleright - updownrange;
        }
      }
      if (steering2 > 10 && steering2 < 170)
      {
        if ( middleleft < middleleftreset)
        {
          middleleft++;
          maxrangeleft = middleleft + updownrange;
          minrangeleft = middleleft - updownrange;
        }
        else if ( middleleft > middleleftreset)
        {
          middleleft--;
          maxrangeleft = middleleft + updownrange;
          minrangeleft = middleleft - updownrange;
        }
      }

      takeabreath = millis();
    }

    shutdowntank++;

  }
  else
  {
    shutdowntank = 0;
  }


  if (shutdowntank > shutdowntimer) {

    leftservo.detach();
    rightservo.detach();
    tankstarted = false;
    takeabreath = 10000;
  }

  steering1 =  map(RemoteXY.joystick_1_y, -100, 100, maxrangeright, minrangeright);    // RIGHT SERVO STEERING
  rightservo.write(steering1);

  steering2 =  map(RemoteXY.joystick_2_y, -100, 100, minrangeleft, maxrangeleft);      // LEFT SERVO STEERING
  leftservo.write(steering2);


  if (RemoteXY.button_1 != 0 && reload + reloadingtime < millis() && health > 0)       // SHOOTING BUTTON
  {
    if (shots < ammo)
    {
      irsend.sendRaw(rawData, 67, 38);
      rightservo.write(100);
      leftservo.write(80);
      delay(100);
      reload = millis();
      shots++;
    }
  }


  if (irrecv.decode(&results)) {

    byte gotshot = 0;
    Serial.println((uint32_t) (results.value), HEX);

    for (int i = 0; i < sizeof(gettingshot); i++) {
      if (results.value == gettingshot[i]) {
        gotshot = 1;
      }
    }

    if (gotshot == 1)
    {
      byte leftright = random(0, 10);
      byte damage = random(mindamage, maxdamage);
      byte rotation = ((health, damage * 10) / 10);

      health = health - damage;
      if (health < 1 || health > 100)
      {
        health = 0;
        disabletank();
      }
      attributedamage(damage, rotation, leftright);
    }

    irrecv.resume();  // Receive the next value
  }
  delay(50);
}

void attributedamage(byte wheredamage, byte howmuch, byte whatside)
{
  if (wheredamage == maxdamage / 2)
  {
    if ( whatside < 5 )
    {
      middleright = middleright - howmuch;
      maxrangeright = middleright + updownrange;
      minrangeright = middleright - updownrange;

      middleleft = middleleft -  howmuch;
      maxrangeleft = middleleft + updownrange;
      minrangeleft = middleleft - updownrange;
    }
    else
    {
      middleright = middleright + howmuch;
      maxrangeright = middleright + updownrange;
      minrangeright = middleright - updownrange;

      middleleft = middleleft + howmuch;
      maxrangeleft = middleleft + updownrange;
      minrangeleft = middleleft - updownrange;
    }
    delay(300);
  }
  else if (wheredamage < maxdamage / 2 )
  {

    if ( whatside < 5 )
    {
      middleright = middleright - howmuch;
      maxrangeright = middleright + updownrange;
      minrangeright = middleright - updownrange;
    }
    else
    {
      middleright = middleright + howmuch;
      maxrangeright = middleright + updownrange;
      minrangeright = middleright - updownrange;
    }
    delay(300);
  }
  else
  {
    if ( whatside < 5 )
    {
      middleleft = middleleft -  howmuch;
      maxrangeleft = middleleft + updownrange;
      minrangeleft = middleleft - updownrange;
    }
    else
    {
      middleleft = middleleft +  howmuch;
      maxrangeleft = middleleft + updownrange;
      minrangeleft = middleleft - updownrange;
    }
    delay(300);
  }
}

void disabletank()
{
  leftservo.detach();
  rightservo.detach();
  takeabreath = 10000;
}
