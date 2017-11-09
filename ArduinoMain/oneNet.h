#include <WiFiLink.h>
#include "EdpPacket.h"
//#include "greeIr.h" 


#define _SSID "Tbxsx"
#define _PASSWD "11111111"


#define CHECK_NO_ERR 0
#define CHECK_FAULT_EDP 1
#define CHECK_CLOSED 2 
#define RECEIVE_OK 10
#define RECEIVE_WAIT 11
#define BUF_SIZE 128


class OneNet{
private:
  WiFiClient client;
//  GreeIR greeIr;
  String server,deviceid,apikey;
  uint16_t port;
  EdpPacketClass edpPkg;
  char recBuf[BUF_SIZE];

  int doSend(unsigned char *data, unsigned short len);
  int doReceive();
public:
  OneNet(){};
  ~OneNet(){};
  
  void init(String _server, uint16_t _port, String _deviceid, String _apikey);
  void reLink();
  
  int sendData(float tem, float hum);
  void clearBuf();
  bool processCmd();
  
};

