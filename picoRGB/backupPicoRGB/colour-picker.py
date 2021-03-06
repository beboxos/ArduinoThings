# SPDX-FileCopyrightText: 2021 Sandy Macdonald
#
# SPDX-License-Identifier: MIT

# This example demonstrates the use of a modifier key to pick the colour of the
# keys' LEDs, as well as the LED sleep functionality.

# Drop the rgbkeypad.py file into your `lib` folder on your `CIRCUITPY` drive.

from rgbkeypad import RgbKeypad, hsv_to_rgb

MODIFIER_KEY = 0

# Set up keypad
keypad = RgbKeypad()

keys = keypad.keys

# Enable LED sleep and set a time of 5 seconds before the LEDs turn off.
# They'll turn back on with a tap of any key!
keypad.led_sleep_enabled = True
keypad.led_sleep_time = 5

# Set up the modifier key. It's 0, or the bottom left key.
modifier_key = keys[MODIFIER_KEY]
modifier_key.modifier = True

# The starting colour (black/off)
rgb = (0, 0, 0)

while True:
    # Always remember to call keybow.update()!
    keypad.update()

    # If the modifier key and any other key are pressed, then set all the
    # keys to the selected colour. The second key pressed picks the colour.
    if modifier_key.held and keypad.any_pressed:
        if len(keypad.get_pressed()) > 1:
            hue = max(keypad.get_pressed()) / 15.0
            rgb = hsv_to_rgb(hue, 1.0, 1.0)

    keypad.set_all(*rgb)
