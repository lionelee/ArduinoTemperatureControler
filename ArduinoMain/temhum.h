#include "dht11.h"

#define OK 0
#define ERROR_CHECKSUM 1
#define ERROR_TIMEOUT 2
#define ERROR_UNKNOWN 3

class Temhum{
private:
  dht11 DHT11;
  int dhtPin=6, chk;
  float tem, hum;
public:
  Temhum(){};
  ~Temhum(){};
  void setPin(int pin);
  int readData();
  float getTemperature();
  float getHumidity();
};

