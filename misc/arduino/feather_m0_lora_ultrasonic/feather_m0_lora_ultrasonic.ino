// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#define deviceName "remote_100"
#define devEUI "100"

#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

// for feather m0  
#define RFM95_CS A3
#define RFM95_RST A4
#define RFM95_INT A5

#define LED 13
#define DONE 12

#define VBATPIN A7

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);


const int trigPin = 10;
const int echoPin = 11;

float duration, distance;

StaticJsonDocument<200> doc;

double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

void setup() 
{

digitalWrite(DONE, LOW);

  pinMode(LED, OUTPUT);
  pinMode(DONE, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

digitalWrite(DONE, LOW);


  Serial.begin(115200);
  /*while (!Serial) {
    delay(1);
  }
  */

  delay(100);

  Serial.println("Feather LoRa TX Test!");

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

void loop()
{

doc["devEUI"] = devEUI;
doc["deviceName"]=deviceName;


  // battery measurement
   float measuredvbat = analogRead(VBATPIN);
measuredvbat *= 2;    // we divided by 2, so multiply back
measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
measuredvbat /= 1024; // convert to voltage

  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!

 // get the ultrasonic distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  doc["distance"] = round2(distance);
  doc["BatV"] = round2(measuredvbat);
  
  Serial.println("Transmitting..."); // Send a message to rf95_server
  
  char radiopacket[80] = "Hello World #      ";
  serializeJson(doc,radiopacket,80);

  
  //itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  //radiopacket[19] = 0;
  
  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket, 80);

  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);               // wait for a second
  
digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);               // wait for a second

  digitalWrite(DONE, HIGH);
}
