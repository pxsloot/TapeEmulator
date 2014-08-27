//
//
//

#ifndef TapeEmulator_h
#define TapeEmulator_h
#include "Arduino.h"

class TapeEmulator
{
  public:
    TapeEmulator(int pin);
    void sendProgram();

  private:
    String checksum(String c);
    void tapesend2Byte(char c[5]);
    void tapesendByte(int c);
    void tapesend(String c);
    void leadsync();
    void datasync();
    void bit0();
    void bit1();
    void khz1();
    void khz2();
    byte getValue(char c);
    int  _out;
    int _khz2interval;
    int _khz1interval;
    int _leadsync_period;
    int _datasync_period;
};


#endif
