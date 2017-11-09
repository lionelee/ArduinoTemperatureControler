#include "temhum.h"

void Temhum::setPin(int pin){
  dhtPin = pin;
}

int Temhum::readData(){
  chk = DHT11.read(dhtPin);
  switch (chk){
    case DHTLIB_OK:
      tem = DHT11.temperature;
      hum = DHT11.humidity;
      return OK;
    case DHTLIB_ERROR_CHECKSUM:
      return ERROR_CHECKSUM;
    case DHTLIB_ERROR_TIMEOUT:
      return ERROR_TIMEOUT;
    default: 
      return ERROR_UNKNOWN;
  }
}

float Temhum::getTemperature(){
  if(chk==OK)
    return tem;
  else 
    return -1;
}

float Temhum::getHumidity(){
  if(chk==OK)
    return hum;
  else 
    return -1;
}


