import board
import busio
import digitalio
import time
import adafruit_rfm9x

import adafruit_hcsr04
sonar = adafruit_hcsr04.HCSR04(trigger_pin=board.D10, echo_pin=board.D11)

#sensor wiring
# ref: https://learn.adafruit.com/ultrasonic-sonar-distance-sensors/python-circuitpython
#Itsy USB or VBat to sensor VCC
#Itsy D5 to sensor trig
#Itsy D6 to 10k resistor - other side to GND
#Sensor echo to 10k resistor - other side to D6
#Itsy and Sensor GND to breadboard ground rail

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
cs = digitalio.DigitalInOut(board.D7)
reset = digitalio.DigitalInOut(board.D9)
rfm9x = adafruit_rfm9x.RFM9x(spi, cs, reset, 915.0)

while True:
    distance = sonar.distance
    sendstring='distance:'+str(distance)
    print(sendstring)
    rfm9x.send(sendstring)
    print('sending:',sendstring)
    led.value = True
    time.sleep(.1)
    led.value = False
    time.sleep(1)