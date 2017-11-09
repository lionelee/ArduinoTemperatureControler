#include "oneNet.h"
#include "fault.h"
#include "Beep.h"
#include <IRremote.h>
extern Fault fault;
extern Beep beep;
IRsend irsend;

unsigned int  rawData16[139] = {8950,4350, 700,450, 750,450, 750,1500, 750,1500, 750,500, 700,450, 750,1550, 700,450, 750,450, 750,450, 700,500, 700,450, 750,450, 750,450, 700,500, 700,450, 750,450, 750,450, 700,500, 700,450, 750,450, 750,1550, 700,450, 750,450, 750,450, 700,500, 700,450, 750,450, 700,1550, 700,500, 700,1600, 650,500, 700,500, 700,1550, 700,500, 700,19700, 600,550, 650,550, 650,550, 650,550, 600,1650, 650,550, 650,500, 700,500, 650,550, 650,1600, 700,500, 650,550, 650,550, 650,1600, 650,550, 650,500, 700,500, 650,550, 650,550, 650,500, 700,500, 650,550, 700,500, 650,500, 700,500, 650,550, 700,500, 650,500, 700,1600, 700,500, 650,500, 700,1600, 650};  // NEC 3200040A
unsigned int code1[2] = {700, 1550};
unsigned int code0[2] = {700, 500};
//unsigned int key[8] = {700,1600, 700,500, 650,500, 700,1600};
//tem 18  16 - 0
//key 130  16 - 9
void temper(unsigned int des[],int begin,int len, int value){
  for (int i = 0 ; i < len ;i++ ){
    if(value & 1){
      des[begin + 2 * i] = code1[0];
      des[begin + 2 * i + 1] = code1[1];
    }else{
     des[begin + 2 * i] = code0[0];
     des[begin + 2 * i + 1] = code0[1];
    }
    value = value >> 1;
  }
}

void sendIR(int tem){
  int dat = tem - 16;
  temper(rawData16,18,4,dat);
  temper(rawData16,8,1,1);
  temper(rawData16,130,4,9 + dat); 
  irsend.sendRaw(rawData16,139,38);
}
void closeAC(){
  int dat = 9;
  temper(rawData16,18,4,dat);
  temper(rawData16,130,4,10);
  temper(rawData16,8,1,0);
  irsend.sendRaw(rawData16,139,38);
}

void OneNet::init(String _server, uint16_t _port, String _deviceid, String _apikey){
  server = _server;
  port = _port;
  deviceid = _deviceid;
  apikey = _apikey;
  
  if (WiFi.status() == WL_NO_WIFI_MODULE_COMM) {
//    Serial.println("Communication failed.");
    while (true);
  }

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
//    Serial.print("SSID:");
    Serial.println(_SSID);
    status = WiFi.begin(_SSID, _PASSWD);
    delay(1000);
  }
  Serial.println("Connwifi");

  if(client.connect(server.c_str(),port)){
    edpPkg.PacketConnect1(deviceid.c_str(),apikey.c_str());
    doSend(edpPkg.GetData(), edpPkg.GetWritepos());
    edpPkg.ClearParameter();
    Serial.println("Connserver");
  }
}

void OneNet::reLink(){
  if(client.connect(server.c_str(),port)){
    edpPkg.PacketConnect1(deviceid.c_str(),apikey.c_str());
    doSend(edpPkg.GetData(), edpPkg.GetWritepos());
    edpPkg.ClearParameter();
//    Serial.println("Relinked to server");
  }
}

int OneNet::doSend(unsigned char *data, unsigned short len){
  client.write(data,len);
  client.flush();
  return 0;
}

int OneNet::doReceive(){
  bool flag = false;
  unsigned int count = 0;
  while(client.available()){
    if(count < BUF_SIZE){
      recBuf[count] = client.read();
    }
    count++;
    flag = true;
  }
//  Serial.println("Count:");
//  Serial.println(count);
//  for(int i = 0; i < count; i++){
//    Serial.print(recBuf[i]);
//  }
//  Serial.println("End");
  if(flag){
    return RECEIVE_OK;  
  }
  return RECEIVE_WAIT;
}

int OneNet::sendData(float tem, float hum){
  if(!client.connected()){
    reLink();
  }
  String send_buf = ",;Temperature,"+String(tem)+";Humidity,"+String(hum)+";";
  Serial.println(send_buf);
  edpPkg.PacketSavedataSimpleString(deviceid.c_str(), send_buf.c_str());
  doSend(edpPkg.GetData(), edpPkg.GetWritepos());
  edpPkg.ClearParameter();
  return 0;
}

void OneNet::clearBuf(){
  memset(recBuf, 0, BUF_SIZE);
}

bool OneNet::processCmd(){
  int num = 0;
  bool flag = 0;
  char numBuf[2];
  char *pstart , *pend;
  char *recEnd = recBuf + BUF_SIZE - 1;
  if(doReceive()==RECEIVE_OK){
    Serial.write(recBuf,BUF_SIZE);
    Serial.println();
//    Serial.println("Begin");
//    Serial.println(*recEnd);
    pstart = recBuf;
    while(pstart<recEnd){
      if((*pstart)=='{')break;
      ++pstart;
    }
//    Serial.println("pstart");
//    Serial.println(*pstart);
    if(pstart == recEnd)return 0;
    pend = pstart;
    while(pend < recEnd){
    if((*pend)=='}')break;  
      ++pend;
    }
//    Serial.println("pend");
//    Serial.println(*pend);
    if(pend == recEnd && pend != '}')return 0;
    char t1 = *(++pstart);
//    Serial.println("t1:");
//    Serial.println(t1);
    ++pstart;
    if((*pstart)!='='){
      return 0;
    }
    beep.enable();
    delay(500);
//    Serial.println("jjj");
    pstart++;
    numBuf[0]=*pstart;
    pstart++;
    numBuf[1]=*pstart;
    int n1 = atoi(numBuf);
    pstart++;
    char t2 = *(++pstart);
    ++pstart;
    if((*pstart)!='='){
      return 0;
    }
    pstart++;
    numBuf[0]=*pstart;
    pstart++;
    numBuf[1]=*pstart;
    int n2 = atoi(numBuf);
    if(t1 == 'T' && t2 == 'k'){
      Serial.println("airconditioner");  
    }
    beep.disable();
    
    clearBuf();
//    Serial.println("Num");
    Serial.println(n1);
    Serial.println(n2);
//    greeIr.sendIR(n1,key);
   if(n2 == 11 ){
      closeAC();
    }
    else {
      sendIR(n1);
     }
  }
  return flag;
}

