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
  "0001", // filename
  "1800", // start address
  "0000", // end address (will be calculated from program data
  //"0E0021C000CDE4050EC0210001CDE40518EE",
  "DD212018CDFE05FE1320F9760000000000000000000000000000000000000000AEB51F858F37", // program data in hex
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
  delay(1000000);
}
