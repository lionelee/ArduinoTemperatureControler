#include "oneNet.h"
#include "temhum.h"
#include "fault.h"
#include "beep.h"

OneNet oneNet;
Fault fault;
Temhum temhum;
Beep beep;

bool sendFlag = 1;

void setup() {
  // Initialize serial
  Serial.begin(9600);
  String server = "jjfaedp.hedevice.com";
  uint16_t port = 876;
  String deviceid = "20176179";
  String apikey = "anSSBCZnB66kuiOKITtb7=SS1mI=";
  oneNet.init(server,port,deviceid,apikey);
}

void loop() {
  oneNet.processCmd();
  if(temhum.readData()==OK){
    oneNet.sendData(temhum.getTemperature(),temhum.getHumidity());               //上传数据
  }
  if(fault.type != FAULT_NONE){                  //如果错误标志被设置
    fault.process();                        //进入错误处理函数
  }
  oneNet.clearBuf();
  delay(5000);
}
