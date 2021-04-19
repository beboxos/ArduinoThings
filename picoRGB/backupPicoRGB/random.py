from rgbkeypad import RgbKeypad
import time
import random
keypad = RgbKeypad()
keys = keypad.keys
delais = 0.1
while True:
    keypad.update()
    i = random.randint(0,15)
    r = random.randint(0,255)
    g = random.randint(0,255)
    b = random.randint(0,255)
    keys[i].set_led(r,g,b)
    time.sleep(delais)
    if keys[3].pressed:
        if delais>0.1:
            delais=delais-0.1
    if keys[15].pressed:
        if delais<1:
            delais=delais+0.1    
    if keys[0].pressed:
        for i in range(0,16):
            keys[i].set_led(0,0,0)
        break