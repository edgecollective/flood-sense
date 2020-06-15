#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include "RTCZero.h" // https://github.com/arduino-libraries/RTCZero

#include <SPI.h>
#include <RH_RF95.h> // https://cdn-learn.adafruit.com/assets/assets/000/031/670/original/RadioHead-1.59.zip?1460574831
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson


#define devEUI "103"
#define deviceName "fml_03"

// for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define VBATPIN A7

const unsigned TX_INTERVAL = 3;


#define LED 13 // built-in LED on arduino board
#define DONE 12 // connect to 'DONE' pin on tpl5110 timer breakout from adafruit

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

#define RTC_SLEEP 1 // 1: sleep; 0: don't sleep

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

//float distance;
uint8_t distance;
float lux;

StaticJsonDocument<200> doc;

double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

RTCZero rtc;

void setup() {


if(RTC_SLEEP) {
      // Initialize RTC
    rtc.begin();
    // Use RTC as a second timer instead of calendar
    rtc.setEpoch(0);
}


   pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  


  
  Serial.begin(115200);

/*
  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  */
  
  //Serial.println("Adafruit VL6180x test!");
  if (! lox.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  
}

int16_t packetnum = 0;  // packet counter, we increment per xmission


void loop() {

VL53L0X_RangingMeasurementData_t measure;

   float measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage


    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  int status = measure.RangeStatus;
  float range = measure.RangeMilliMeter;
  
  float lux = 0.0;  // is there a lux value we can get from this sensor?
  

  doc["dist"] = range;
  doc["lux"] = round2(lux);
  doc["status"] = status;
  doc["batt"] = measuredvbat;
  doc["devEUI"] = devEUI;
  doc["deviceName"] = deviceName;

  Serial.println("Transmitting..."); // Send a message to rf95_server
  
  char radiopacket[100] = "Hello World #      ";
  serializeJson(doc,radiopacket,100);

  
  Serial.print("JSON to send: "); Serial.println(radiopacket);
  
  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket, 100);

  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();

  Serial.println("Sent.");
  
rf95.sleep();

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);               // wait for a second
  
digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);               // wait for a second

  delay(1000);

  if(RTC_SLEEP) {

/*
pinMode(0,INPUT_PULLUP);
    pinMode(1,INPUT_PULLUP);
    pinMode(A0,INPUT_PULLUP);
    pinMode(A1,INPUT_PULLUP);
    pinMode(A2,INPUT_PULLUP);
    pinMode(A3,INPUT_PULLUP);
    pinMode(A4,INPUT_PULLUP);
    pinMode(A5,INPUT_PULLUP);
    pinMode(0,INPUT_PULLUP);
    pinMode(1,INPUT_PULLUP);
    pinMode(5,INPUT_PULLUP);
    pinMode(9,INPUT_PULLUP);
    pinMode(10,INPUT_PULLUP);
    pinMode(11,INPUT_PULLUP);
    pinMode(12,INPUT_PULLUP);
    */
    //pinMode(13,INPUT_PULLUP);
    
            // Sleep for a period of TX_INTERVAL using single shot alarm
            rtc.setAlarmEpoch(rtc.getEpoch() + TX_INTERVAL);
            rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
            rtc.attachInterrupt(alarmMatch);
            
            // USB port consumes extra current
            USBDevice.detach();
           
            // Enter sleep mode
            rtc.standbyMode();
            
            
            // Reinitialize USB for debugging
            USBDevice.init();
            USBDevice.attach();
            }
            
}

void alarmMatch()
{

}
