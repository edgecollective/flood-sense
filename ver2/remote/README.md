# Remote 

## Required Arduino libraries

[RadioHead](https://cdn-learn.adafruit.com/assets/assets/000/031/670/original/RadioHead-1.59.zip?1460574831)

[ArduinoJson](https://github.com/bblanchon/ArduinoJson)


## Hardware

- Feather M0 micro
- Ultrasonic Sensor
- LoRa Breakout 
- TPL5110 Timer

(Also works without Timer, for testing)

## Wiring

**LoRa Breakout -- Feather M0**

- CS -- A3
- RST -- A4
- INT -- A5

**Ultrasonic -- Feather M0**

- TRIG -- 10
- ECHO -- 11 (via voltage divider)

**Timer -- Feather M0**

- DRV -- BAT
- DONE -- 12




