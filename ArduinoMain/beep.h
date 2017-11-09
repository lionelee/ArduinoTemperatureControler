class Beep{
private:
  bool beepStat;
  int beepPin=5;
public:
  Beep();
  Beep(int pin);
  ~Beep(){};
  void setPin(int pin);
  void enable();
  void disable();  
  bool getStat();
};

