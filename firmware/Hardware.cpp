#include "Hardware.h"

// Initialize the pins
void initHardware() {
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
}

// Control the LED
void setLedState(bool state) {
  digitalWrite(PIN_LED, state ? HIGH : LOW);
}

// Read the sensor
int readLDR() {
  return analogRead(PIN_LDR);
}

// Read the button
bool readButton() {
  return digitalRead(PIN_BTN);
}

// Control the buzzer (active)
void beep(int freq, int duration) {
  digitalWrite(PIN_BUZZER, HIGH); 
  delay(duration);                
  digitalWrite(PIN_BUZZER, LOW);  
}