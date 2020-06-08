import board
import busio
import digitalio
import time
import adafruit_rfm9x

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
cs = digitalio.DigitalInOut(board.D7)
reset = digitalio.DigitalInOut(board.D9)
rfm9x = adafruit_rfm9x.RFM9x(spi, cs, reset, 915.0)

while True:
    print('listening')
    packet = rfm9x.receive()  # Wait for a packet to be received (up to 0.5 seconds)
    if packet is not None:
        print(packet)
        led.value = True
        time.sleep(.1)
        led.value = False