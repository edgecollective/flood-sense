import board
import busio
import digitalio
import time
import adafruit_rfm9x

spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
cs = digitalio.DigitalInOut(board.D7)
reset = digitalio.DigitalInOut(board.D9)
rfm9x = adafruit_rfm9x.RFM9x(spi, cs, reset, 915.0)

while True:
    print('listening')
    rfm9x.receive()
    time.sleep(1)
