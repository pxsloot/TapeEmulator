// cassette emulator for mpf-1
//
// bit 0 = 8 cycles @ 2KHz + 2 cycles @ 1KHz
// bit 1 = 4 cycles @ 2KHz + 4 cycles @ 1KHz
// byte = start bit 0 + 8 data bits + stop bit 1
// file = 4s 1KHz
//        + 2bytes filename
//        + 2bytes start addr
//        + 2 bytes stop addr
//        + 1 bytes checksum
//        + 2s 2KHz
//        + data
//        + 2s 2KHz

struct Program {
  char filename[5];
  char start_address[5];
  char stop_address[5];
  String data;
};

Program p = {
  "0001", // filename
  "1800", // start address
  "0000", // end address (will be calculated from program data)
  //"0E0021C000CDE4050EC0210001CDE40518EE",
  "DD212018CDFE05FE1320F9760000000000000000000000000000000000000000AEB51F858F37", // program data in hex
};

int out = 13;
int khz1interval = 500;
int khz2interval = 250;
int leadsync_period = 4000;
int datasync_period = 2000;

void setup(){
  pinMode(out, OUTPUT);
  digitalWrite(out, LOW);
  //noInterrupts();
}

void loop(){
  sendProgram();
  delay(100000000);
}

void sendProgram() {
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
}

String checksum(String c){
  int sum = 0;
  for (int i=0; i < c.length(); i+=2){
    sum += ((getValue(c[i]) << 4) + getValue(c[i+1]));
  }
  return String(sum & 0xFF,HEX);
}


void tapesend2Byte(char c[5]){
  tapesendByte((getValue(c[2])<<4) + getValue(c[3]));
  tapesendByte((getValue(c[0])<<4) + getValue(c[1]));
}

void tapesendByte(int c){
  // start bit
  bit0();
  for (int i=0; i<8;i++){
    (bitRead(c, i)) ? bit1() : bit0();
  }
  // stop bit
  bit1();
}

void tapesend(String c){
  for (int i=0; i < c.length(); i+=2){
    tapesendByte((getValue(c[i])<<4) + getValue(c[i+1]));
  }
}

void leadsync(){
  unsigned long startMillis = millis();
  unsigned long currentMillis = millis();
  while (currentMillis - startMillis < leadsync_period ) {
    currentMillis = millis();
    khz1();
  }
}

void datasync(){
  unsigned long startMillis = millis();
  unsigned long currentMillis = millis();
  while (currentMillis - startMillis < datasync_period ) {
    currentMillis = millis();
    khz2();
  }
}

void bit0() {
  for (int n=0; n<8; n++) { khz2(); }
  for (int n=0; n<2; n++) { khz1(); }
}

void bit1() {
  for (int n=0; n<4; n++) { khz2(); }
  for (int n=0; n<4; n++) { khz1(); }
}

void khz1() {
  digitalWrite(out, HIGH);
  delayMicroseconds(khz1interval);
  digitalWrite(out, LOW);
  delayMicroseconds(khz1interval);
}

void khz2() {
  digitalWrite(out, HIGH);
  delayMicroseconds(khz2interval);
  digitalWrite(out, LOW);
  delayMicroseconds(khz2interval);
}

byte getValue(char c){
  if(c >= '0' && c <= '9')
    return (byte)(c-'0');
  else if (c >= 'a' && c <= 'f')
    return (byte)(c-'a'+10);
  else
    return (byte)(c-'A'+10);
}
