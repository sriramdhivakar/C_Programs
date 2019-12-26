#!/usr/bin/python3
import mraa  
import time   

led = mraa.Gpio(61)          
switch = mraa.Gpio(35)

led.dir(mraa.DIR_OUT)        
switch.dir(mraa.DIR_IN)

while True:           
    if switch.read():
        led.write(0)
        time.sleep(1)
    else:
        led.write(1)
        time.sleeo(1)
     
