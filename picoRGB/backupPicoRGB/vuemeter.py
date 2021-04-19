from rgbkeypad import RgbKeypad
import time
import random
keypad = RgbKeypad()
keys = keypad.keys

# vu meter simulation
delais = 0.1
while True:
    keypad.update()
    #if keys[0].pressed:
    #    for i in range(0,50):
    x = random.randint(0, 5)
    y = random.randint(4, 8)
    z = random.randint(8, 12)
    w = random.randint(12,16)
    if x>0:
        keys[0].set_led(0,255,0)
    if x>1:
        keys[1].set_led(0,255,0)
    if x>2:
        keys[2].set_led(255,255,0)
    if x>3:
        keys[3].set_led(255,0,0)
    if y>4:
        keys[4].set_led(0,255,0)
    if y>5:
        keys[5].set_led(0,255,0)
    if y>6:
        keys[6].set_led(255,255,0)
    if y>7:
        keys[7].set_led(255,0,0)
    if z>8:
        keys[8].set_led(0,255,0)
    if z>9:
        keys[9].set_led(0,255,0)
    if z>10:
        keys[10].set_led(255,255,0)
    if z>11:
        keys[11].set_led(255,0,0)
    if w>12:
        keys[12].set_led(0,255,0)
    if w>13:
        keys[13].set_led(0,255,0)
    if w>14:
        keys[14].set_led(255,255,0)
    if w>15:
        keys[15].set_led(255,0,0)
    time.sleep(delais)
    for i in range(0,16):
        keys[i].set_led(0,0,0)
    if keys[3].pressed:
        if delais>0.1:
            delais=delais-0.1
    if keys[15].pressed:
        if delais<1:
            delais=delais+0.1
    if keys[0].pressed:
        break

