#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

#define PIN_LDR  34
#define PIN_LED  36
#define PIN_BTN  21
#define PIN_BUZZER  14

void initHardware();
void setLedState(bool state);
int readLDR();
bool readButton();
void beep(int freq, int duration);

#endif