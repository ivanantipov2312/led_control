// For OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pins for RGB led
const uint8_t RGB_RED_PIN = 23;
const uint8_t RGB_GREEN_PIN = 19;
const uint8_t RGB_BLUE_PIN = 18;

// Pins for leds that show current mode
const uint8_t STATE_RED_PIN = 25;
const uint8_t STATE_GREEN_PIN = 26;
const uint8_t STATE_BLUE_PIN = 27;
const uint8_t STATE_IDLE_PIN = 32;

// Input pins
const uint8_t BUTTON_PIN = 33;
const uint8_t POTENTIOMETER_PIN = 34;

// For state leds
int led_state = -1;

// Button
int button_state = 0;
int last_button_state = 0;
unsigned long last_debounce_time = 0;
const unsigned long DEBOUNCE_TIME = 50;

// LED
int r = 0;
int g = 0;
int b = 0;
int brightness = 0;

// OLED
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Helper functions
void draw_string(const char *str, int cursor_x = 0, int cursor_y = 0, bool clear_screen = true) {
  if (clear_screen)
    display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(cursor_x, cursor_y);

  // Print string char by char
  for (int i = 0; i < strlen(str) + 1; i++) {
    display.write(str[i]);
  }
  display.write('\n');
  display.display();
}

void switch_states() {
  led_state++;
  led_state %= 4;

  switch (led_state) {
    // Initial (BLUE -> IDLE)
    case 0:
      digitalWrite(STATE_IDLE_PIN, HIGH);
      digitalWrite(STATE_BLUE_PIN, LOW);
      b = brightness; // Confirm blue level from previous state
      draw_string("IDLE MODE");
      break;
    // IDLE -> RED
    case 1:
      digitalWrite(STATE_RED_PIN, HIGH);
      digitalWrite(STATE_IDLE_PIN, LOW);
      draw_string("RED MODE");
      // Idle changes nothing so no assignments
      break;
    // RED -> GREEN
    case 2:
      digitalWrite(STATE_GREEN_PIN, HIGH);
      digitalWrite(STATE_RED_PIN, LOW);
      r = brightness; // Confirm red level from previous state
      draw_string("GREEN MODE");
      break;
    // GREEN -> BLUE
    case 3:
      digitalWrite(STATE_BLUE_PIN, HIGH);
      digitalWrite(STATE_GREEN_PIN, LOW);
      g = brightness; // Confirm green level from previous state
      draw_string("BLUE MODE");
      break;
  }
}

void setup() {
  Serial.begin(9600);

  // OLED initial setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Enable onboard led, just for fun
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Set all of RGB led pins to output mode
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  // Set all of state led pins to output mode
  pinMode(STATE_RED_PIN, OUTPUT);
  pinMode(STATE_GREEN_PIN, OUTPUT);
  pinMode(STATE_BLUE_PIN, OUTPUT);
  pinMode(STATE_IDLE_PIN, OUTPUT);

  // Button and potentiometer are in input mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_PIN, INPUT);

  // Initial state, so enable idle pin
  digitalWrite(STATE_IDLE_PIN, HIGH);
}

void loop() {
  display.setTextSize(1);

  // Button input
  int reading = digitalRead(BUTTON_PIN);

  // Read potentiometer input and convert it from range 0..4095 to range 0..255
  brightness = analogRead(POTENTIOMETER_PIN);
  brightness = map(brightness, 0, 4095, 0, 255);

  // If state has changed
  if (reading != last_button_state) {
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > DEBOUNCE_TIME) {
    if (reading != button_state) {
      button_state = reading;

      // If button is released
      if (button_state == HIGH) {
        switch_states();
        char status[32];
        sprintf(status, "RED: %d\nGREEN: %d\nBLUE: %d", r, g, b);
        draw_string(status, 0, 10, false);
      }
    }
  }

  // Write r, g and b to our RGB LED
  analogWrite(RGB_RED_PIN, r);
  analogWrite(RGB_GREEN_PIN, g);
  analogWrite(RGB_BLUE_PIN, b);

  last_button_state = reading;
}
