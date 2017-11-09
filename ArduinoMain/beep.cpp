#include "beep.h"
#include <Arduino.h>

Beep::Beep(){
  pinMode(beepPin, OUTPUT); 
}

Beep::Beep(int pin):beepPin(pin){
  pinMode(pin,OUTPUT);
}

void Beep::setPin(int pin){
  beepPin = pin;
}

void Beep::enable(){
  digitalWrite(beepPin, 1);
  beepStat = true;
}

void Beep::disable(){
  digitalWrite(beepPin,0);
  beepStat = false;
}

bool Beep::getStat(){
  return beepStat;
}

