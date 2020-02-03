#include "ClickButton.h"
#include <EEPROM.h>

// EPROM memory where Device ID is stored
#define DEVICE_ID 0

// Arduino/Button/Relay mapping
//
// A1: first Mega 2560
const int A1_BTN_P3_Pin = 33; 
const int A1_BTN_P4_Pin = 41;
const int A1_RLY_40A_Pin = 10; int A1_RLY_40A_State = 0;
const int A1_RLY_8M1_Pin = 7; int A1_RLY_8M1_State = 0;
const int A1_RLY_4M1_Pin = 8; int A1_RLY_4M1_State= 0;
//
// A2: second Mega 2560
const int A2_BTN_P1_Pin = 34; 
const int A2_BTN_P2_Pin = 28;
const int A2_RLY_8M2_Pin = 7; int A2_RLY_8M2_State = 0;
const int A2_RLY_4M2_Pin = 8; int A2_RLY_4M2_State = 0;


// Buttons object definitions
ClickButton BTN_P1(A2_BTN_P1_Pin, LOW, CLICKBTN_PULLUP);
ClickButton BTN_P2(A2_BTN_P2_Pin, LOW, CLICKBTN_PULLUP);
ClickButton BTN_P3(A1_BTN_P3_Pin, LOW, CLICKBTN_PULLUP);
ClickButton BTN_P4(A1_BTN_P4_Pin, LOW, CLICKBTN_PULLUP);
// ...with also some "temp" variables
int BTN_P1_clicks = 0;
int BTN_P2_clicks = 0;
int BTN_P3_clicks = 0;
int BTN_P4_clicks = 0;

int thisdev = -1; // to hold the ID of running device

void setup()
{
  // this is my main "output/debug console"
  Serial.begin (9600);
  
  // Let's check, in the EEPROM, which is my ID
  // BTW:
  //  - ID: 1   => MASTER
  //  - ID: 2   => SLAVE
  //  - ID: 100 => SLAVE
  thisdev = EEPROM.read(DEVICE_ID);

  // some "debug" message...
  Serial.print("Starting on [");
  Serial.print(thisdev);
  Serial.println("]");

  switch (thisdev) {
    case 1:
      // Let's setup relay PINs as OUTPUT
      Serial.println("Initializing A1 OUTPUT PINs...");
      pinMode(A1_RLY_40A_Pin, OUTPUT);  
      pinMode(A1_RLY_8M1_Pin, OUTPUT);
      pinMode(A1_RLY_4M1_Pin, OUTPUT);
      //
      Serial.println("Initializing A1 BUTTONs...");
      // Setup button timers (all in milliseconds / ms)
      // <button>.debounceTime   => Debounce timer in ms
      // <button>.multiclickTime => Time limit for multi clicks
      // <button>.longClickTime  => time until "held-down clicks" register
      BTN_P3.debounceTime   = 20;
      BTN_P3.multiclickTime = 350;
      BTN_P3.longClickTime  = 1200;
      //
      BTN_P4.debounceTime   = 20;
      BTN_P4.multiclickTime = 350;
      BTN_P4.longClickTime  = 1200;
      break;
    case 2:
      // Let's setup relay PINs as OUTPUT
      Serial.println("Initializing A2 OUTPUT PINs...");
      pinMode(A2_RLY_8M2_Pin, OUTPUT);
      pinMode(A2_RLY_4M2_Pin, OUTPUT);
      //
      Serial.println("Initializing A2 BUTTONs...");
      // Setup button timers (all in milliseconds / ms)
      // <button>.debounceTime   => Debounce timer in ms
      // <button>.multiclickTime => Time limit for multi clicks
      // <button>.longClickTime  => time until "held-down clicks" register
      BTN_P1.debounceTime   = 20;
      BTN_P1.multiclickTime = 350;
      BTN_P1.longClickTime  = 1200;
      //
      BTN_P2.debounceTime   = 20;
      BTN_P2.multiclickTime = 350;
      BTN_P2.longClickTime  = 1200;
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
      Serial.print("Hei! You should _NEVER_ get here! [");
      Serial.print(thisdev);
      Serial.println("]");
      break;
  }
}


void loop()
{
  switch (thisdev) {
    case 1:
      // Update button state
      BTN_P3.Update();
      BTN_P4.Update();
      //
      // Save clicks count, as click codes are reset at next Update()
      if (BTN_P3.clicks != 0) {
        BTN_P3_clicks = BTN_P3.clicks;
        Serial.print("BTN_P3 has been CLICKSed...(");Serial.print(BTN_P3_clicks);Serial.println(")");
      }
      if (BTN_P4.clicks != 0) {
        BTN_P4_clicks = BTN_P4.clicks;
        Serial.print("BTN_P4 has been CLICKSed...(");Serial.print(BTN_P4_clicks);Serial.println(")");
      }
      //
      // Simply toggle relay on single clicks
      // (Cant use LEDfunction like the others here,
      //  as it would toggle on and off all the time)
      if(BTN_P3.clicks == 1) {
        Serial.println("BTN_P3 has been single-pressed [L1 switch]");
        A1_RLY_8M1_State = !A1_RLY_8M1_State;
      }
      if(BTN_P4.clicks == 1) {
        Serial.println("BTN_P4 has been single-pressed [L3 switch]");
        A1_RLY_4M1_State = !A1_RLY_4M1_State;
      }
      // Long press on P3 will turn all lights on
      if(BTN_P3.clicks <= -1) {
        Serial.println("BTN_P3 has been long-pressed [L1+L3 on]");
        A1_RLY_8M1_State = HIGH;
        A1_RLY_4M1_State = HIGH;
      }
      // Long press on P4 will turn all lights off
      if(BTN_P4.clicks <= -1) {
        Serial.println("BTN_P4 has been long-pressed [L1+L3 off]");
        A1_RLY_8M1_State = LOW;
        A1_RLY_4M1_State = LOW;
      }
      if(BTN_P3.clicks == 2) {
        Serial.println("BTN_P3 has been double-pressed [40A switch]");
        A1_RLY_40A_State = !A1_RLY_40A_State;
      }
      // update the Relay status
      digitalWrite(A1_RLY_40A_Pin, A1_RLY_40A_State);
      digitalWrite(A1_RLY_8M1_Pin, A1_RLY_8M1_State);
      digitalWrite(A1_RLY_4M1_Pin, A1_RLY_4M1_State);
      break;
    case 2:
      // Update button state
      BTN_P1.Update();
      BTN_P2.Update();
      //
      // Save clicks count, as click codes are reset at next Update()
      if (BTN_P1.clicks != 0) {
        BTN_P1_clicks = BTN_P1.clicks;
        Serial.print("BTN_P1 has been CLICKSed...(");Serial.print(BTN_P1_clicks);Serial.println(")");
      }
      if (BTN_P2.clicks != 0) {
        BTN_P2_clicks = BTN_P2.clicks;
        Serial.print("BTN_P2 has been CLICKSed...(");Serial.print(BTN_P2_clicks);Serial.println(")");
      }
      //
      // Simply toggle relay on single clicks
      // (Cant use LEDfunction like the others here,
      //  as it would toggle on and off all the time)
      if(BTN_P1.clicks == 1) {
        Serial.println("BTN_P1 has been single-pressed [L2 switch]");
        A2_RLY_8M2_State = !A2_RLY_8M2_State;
      }
      if(BTN_P2.clicks == 1) {
        Serial.println("BTN_P2 has been single-pressed [L4 switch]");
        A2_RLY_4M2_State = !A2_RLY_4M2_State;
      }
      // Long press on P1 will turn all lights on
      if(BTN_P1.clicks <= -1) {
        Serial.println("BTN_P1 has been long-pressed [L2+L4 on]");
        A2_RLY_8M2_State = HIGH;
        A2_RLY_4M2_State = HIGH;
      }
      // Long press on P4 will turn all lights off
      if(BTN_P2.clicks <= -1) {
        Serial.println("BTN_P2 has been long-pressed [L2+L4 off]");
        A2_RLY_8M2_State = LOW;
        A2_RLY_4M2_State = LOW;
      }
      digitalWrite(A2_RLY_8M2_Pin, A2_RLY_8M2_State);
      digitalWrite(A2_RLY_4M2_Pin, A2_RLY_4M2_State);
      break;
    default:
      break;
  }
}
