#Funny spiral demo by BeBoX

from rgbkeypad import RgbKeypad
import time
keypad = RgbKeypad()
keys = keypad.keys
y=0
speed=0.1
colors=[255,0,0,0,255,0,0,0,255,255,255,255]
for c in range(0,12,3):    
    for i in range(0,4):
        keypad.update()
        keys[i].set_led(colors[c],colors[c+1],colors[c+2])
        time.sleep(speed)
        keys[i].set_led(0,0,0)
    for i in range(7,15,4):
        keypad.update()
        keys[i].set_led(colors[c],colors[c+1],colors[c+2])
        time.sleep(speed)
        keys[i].set_led(0,0,0)
    for i in range(15,11,-1):
        keypad.update()
        keys[i].set_led(colors[c],colors[c+1],colors[c+2])
        time.sleep(speed)
        keys[i].set_led(0,0,0)
    for i in range(8,4,-4):
        keypad.update()
        keys[i].set_led(colors[c],colors[c+1],colors[c+2])
        time.sleep(speed)
        keys[i].set_led(0,0,0)
    for i in range(4,7):
        keypad.update()
        keys[i].set_led(colors[c],colors[c+1],colors[c+2])
        time.sleep(speed)
        keys[i].set_led(0,0,0)
    for i in range(10,8,-1):
        keypad.update()
        keys[i].set_led(colors[c],colors[c+1],colors[c+2])
        time.sleep(speed)
        keys[i].set_led(0,0,0)
      
