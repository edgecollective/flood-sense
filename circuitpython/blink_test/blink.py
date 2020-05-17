
import board
import busio
import digitalio
import time
import adafruit_rfm9x

led = digitalio.DigitalInOut(board.D7)
led.direction = digitalio.Direction.OUTPUT

while True:
    led.value = True
    time.sleep(.5)
    led.value = False
