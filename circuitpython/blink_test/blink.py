
import board
import busio
import digitalio
import time
import adafruit_rfm9x

led = digitalio.DigitalInOut(board.D7)
#led = digitalio.DigitalInOut(board.D9)
#led = digitalio.DigitalInOut(board.SCK)
#led = digitalio.DigitalInOut(board.MOSI)
#led = digitalio.DigitalInOut(board.MISO)

led.direction = digitalio.Direction.OUTPUT

while True:
    led.value = True
    time.sleep(.5)
    led.value = False
