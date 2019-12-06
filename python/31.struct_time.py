#!/usr/bin/python3
import time
t = (2015, 12, 31, 10, 39, 45, 1, 48, 0)
t = time.mktime(t)
print (time.strftime("%b %d %Y %H:%M:%S", time.localtime(t)))
print (time.strftime("%H:%M:%S", time.localtime(t)))
