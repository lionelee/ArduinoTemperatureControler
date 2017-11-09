#include "fault.h"
#include "onenet.h"

extern OneNet oneNet;

void Fault::process(void)
{    
  switch(type){
    case FAULT_REBOOT:                      //死机重启的处理
      type = FAULT_NONE;                 //死机重启后，设备会被复位，所以是重新初始化
      break;
    case FAULT_EDP:               //协议出错的处理
      type = FAULT_NONE;
      oneNet.reLink();                  //连接IP
      break;
    default:break;
  }
}

