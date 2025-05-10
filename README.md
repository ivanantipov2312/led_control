# led_control
My arduino porject for controlling red, blue and green levels of RGB LED.

# Hardware
- 4 LEDs for displaying different modes (red, green, blue and white for idle)
- OLED screen (mine is 128x64 SSD1306)
- RGB LED
- Potentiometer (mine is 10K potentiometer)
- Resistors 220R for LEDs (no resistor for potentiometer or OLED was used)
- Push button

# Basic usage
Programm has following modes of operation
- Idle mode: nothing happens, just for to see what color is with the current values for r, g and b
- Red mode: setting value for red (0-255)
- Green mode: setting value for green (0-255)
- Blue mode: setting value for blue (0-255)

If the current mode is red, then OLED displays text 'RED MODE' and red LED is on and etc.
