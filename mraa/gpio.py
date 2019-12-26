#!/usr/bin/python3
import mraa 
import time 
gpio_1 = mraa.Gpio (61)
#initialise gpio 61  (D4 – Ref: Schematic)                 
gpio_2 = mraa.Gpio(62)          
# initialise gpio 62  (D7)
gpio_3 = mraa.Gpio (63)
#initialise gpio 63  (D17)
gpio_1.dir (mraa.DIR_OUT)
#set gpio 61 to output                                       
gpio_2.dir(mraa.DIR_OUT)        
# set gpio 62 to output                              
gpio_3.dir(mraa.DIR_OUT)        
#set gpio 63 to output                                                                                                 
# toggle both gpio's
while True:
    gpio_1.write (1)
    gpio_2.write (0)
    gpio_3.write (1)
    time.sleep (1)
    gpio_1.write (0)
    gpio_2.write (1)
    gpio_3.write (0)
    time.sleep (1)
    gpio_1.write (0) 
    gpio_2.write (0) 
    gpio_3.write (1) 
    time.sleep (1)
