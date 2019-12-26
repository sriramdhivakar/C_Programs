#!/usr/bin/python3
import os

# Changing a directory to "/home/newdir"
os.chdir("/home/sriram/BBB")
print(os.getcwd())
fo = open("sriram.txt","wb+")

fo.close()
os.rmdir("/home/sriram/ram")
