import board
import busio
import digitalio
import time
import adafruit_rfm9x
import microcontroller

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

# wiring from lora breakout to itsybitsy_m4:
# (lora breakout --> itsybitsy_m4)
# VIN --> 3V
# GND --> GND
# SCK --> SCK
# MISO --> MISO
# MOSI --> MOSI
# CS --> D7
# RST --> D9

spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
cs = digitalio.DigitalInOut(board.D7)
reset = digitalio.DigitalInOut(board.D9)
rfm9x = adafruit_rfm9x.RFM9x(spi, cs, reset, 915.0)

while True:
    temp = microcontroller.cpu.temperature
    sendstring = 'temperature:'+str(temp)
    rfm9x.send(sendstring)
    print('sending:',sendstring)
    led.value = True
    time.sleep(.1)
    led.value = False
    time.sleep(1)