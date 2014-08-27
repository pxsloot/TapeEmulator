//
// TapeEmulator.cpp
//

#include "Arduino.h"
#include "TapeEmulator.h"

TapeEmulator::TapeEmulator(int pin)
{
//  pinMode(pin, OUTPUT);
//  _pin = pin;
}

int _out = 13;
int _khz2interval = 250;
int _khz1interval = 500;
int _leadsync_period = 3000;
int _datasync_period = 2000;
unsigned long _currentMillis = 0;

String TapeEmulator::checksum(String c){
  int sum = 0;
  for (int i=0; i < c.length(); i+=2){
    sum += ((getValue(c[i]) << 4) + getValue(c[i+1]));
  }
  return String(sum & 0xFF,HEX);
}

void TapeEmulator::tapesend2Byte(char c[5]){
  tapesendByte((getValue(c[2])<<4) + getValue(c[3]));
  tapesendByte((getValue(c[0])<<4) + getValue(c[1]));
}

void TapeEmulator::tapesendByte(int c){
  // start bit
  bit0();
  for (int i=0; i<8;i++){
    (bitRead(c, i)) ? bit1() : bit0();
  }
  // stop bit
  bit1();
}

void TapeEmulator::tapesend(String c){
  for (int i=0; i < c.length(); i+=2){
    tapesendByte((getValue(c[i])<<4) + getValue(c[i+1]));
  }
}

void TapeEmulator::leadsync(){
  unsigned long startMillis = millis();
  unsigned long currentMillis = millis();
  while (currentMillis - startMillis < _leadsync_period ) {
    currentMillis = millis();
    TapeEmulator::khz1();
  }
}

void TapeEmulator::datasync(){
  unsigned long startMillis = millis();
  unsigned long currentMillis = millis();
  while (currentMillis - startMillis < _datasync_period ) {
    currentMillis = millis();
    TapeEmulator::khz2();
  }
}


void TapeEmulator::bit0() {
  for (int n=0; n<8; n++) { TapeEmulator::khz2(); }
  for (int n=0; n<2; n++) { TapeEmulator::khz1(); }  
}

void TapeEmulator::bit1() {
  for (int n=0; n<4; n++) { TapeEmulator::khz2(); }
  for (int n=0; n<4; n++) { TapeEmulator::khz1(); }  
}

void TapeEmulator::khz1() {
  digitalWrite(_out, HIGH);
  delayMicroseconds(_khz1interval);
  digitalWrite(_out, LOW);
  delayMicroseconds(_khz1interval);
}

void TapeEmulator::khz2() {
  digitalWrite(_out, HIGH);
  delayMicroseconds(_khz2interval);
  digitalWrite(_out, LOW);
  delayMicroseconds(_khz2interval);
}

byte TapeEmulator::getValue(char c){
  if(c >= '0' && c <= '9')
    return (byte)(c - '0');
  else if (c >= 'a' && c <= 'f')
    return (byte)(c-'a'+10);
  else
    return (byte)(c-'A'+10);
}
