// cassette emulator for mpf-1
//
// bit 0 = 8 cycles @ 2KHz + 2 cycles @ 1KHz
// bit 1 = 4 cycles @ 2KHz + 4 cycles @ 1KHz

struct Program {
  char filename[5];
  char start_address[5];
  char stop_address[5];
  String data;
};

Program p = {
  "0001",
  "1800",
  "0000",
  //"0E0021C000CDE4050EC0210001CDE40518EE",
  "DD212018CDFE05FE1320F9760000000000000000000000000000000000000000AEB51F858F37",
};

int out=13;
int pinState = LOW;

void setup(){
  pinMode(out, OUTPUT);
  digitalWrite(out, pinState);
  //noInterrupts();
  unsigned long startMillis = millis();
}

void loop(){
  int stop_address = (getValue(p.start_address[3]))
                   + (getValue(p.start_address[2]) << 4)
                   + (getValue(p.start_address[1]) << 8)
                   + (getValue(p.start_address[0]) << 12)
                   + (p.data.length()/2) - 1;

  String(stop_address,HEX).toCharArray(p.stop_address, 5);
  //tapeHeader();
  leadsync();
  // filename
  tapesend2Byte(p.filename);
  // start_addr
  tapesend2Byte(p.start_address);
  // stop_addr
  tapesend2Byte(p.stop_address);
  // checksum
  tapesend(checksum(p.data));
  // sync
  datasync();
  // data
  tapesend(p.data);
  // 2 sec sync
  datasync();  
  delay(1000000);
}




