typedef enum{
  FAULT_NONE = 0,   //无错误
  FAULT_REBOOT,   //死机重启错误
  FAULT_NODEVICE, //设备错误
  FAULT_EDP,      //EDP协议错误
} FAULT_TYPE;

class Fault{
public:
  Fault(){};
  ~Fault(){};
  void process(void);
  unsigned char type;
};
