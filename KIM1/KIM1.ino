// cassette emulator for kim-1
//
// ref: http://mamedev.org/source/src/mess/drivers/kim1.c
//      http://en.wikipedia.org/wiki/KIM-1#Tape_format
//
// The KIM-1 stores data on cassette using 2 frequencies: ~3700Hz (high) and ~2400Hz
// (low). A high tone is output for 9 cycles and a low tone for 6 cycles. A logic bit
// is encoded using 3 sequences of high and low tones. It always starts with a high
// tone and ends with a low tone. The middle tone is high for a logic 0 and low for
// 0 logic 1.
//
// each bit is 3 tones
// bit 0 = 3700Hz + 3700Hz + 2400Hz
// bit 1 = 3700Hz + 2400Hz + 2400Hz
//
// file = 100 bytes 0x16
//        + 1 byte 0x2A
//        + record id number
//        + start addr (2 char low byte, 2 char high byte)
//        + end addr (2 char low byte, 2 char high byte)
//        + data (each byte stored as 2 ASCII characters: B5 is stored as 'B' and '5')
//        + 1 byte 0x2F
//        + 2 byte checksum
//        + 2 bytes 0x04

struct Program {
  char filename[5];
  char start_address[5];
  char stop_address[5];
  String data;
};

Program p = {
  "0001", // filename
  "0200", // start address
  "0000", // end address (will be calculated from program data)
  "A20DBDCC0295D5CA10F8A205A001F818"
  "B5D575D795D5CA8810F6B5D81002A999"
  "75D595D5CA10E5A5D5100DA90085E2A2"
  "0295D595DBCA10F938A5E0E5DD85E0A2"
  "01B5DEE90095DECA10F7B00CA900A203"
  "95DDCA10FB20BDA5DEA6DF09F0A4E1F0"
  "20F09CF0A4A2FEA05A18A5D96905A5D8"
  "6900B004A2ADA0DE98A4E2F004A5D5A6"
  "D685FB86FAA5D9A6D8100538A900E5D9"
  "85F9A90285E3D8201F1F206A1FC913F0"
  "C0B00320AD02C6E3D0EDF0B7C90A9005"
  "490F85E160AAA5DDF0FA86DDA5DD38F8"
  "E90585DCA900E90085DB604501009981"
  "009997020800000101", // program data in hex
};

int out = 13;

int interval_2400 = 208; // (1/2400 * 1000000)/2 microseconds
int interval_3700 = 135; // (1/3700 * 1000000)/2 microseconds

// lunar lander
String data = "A20DBDCC0295D5CA10F8A205A001F818B5D575D795D5CA8810F6B5D81002A99975D595D5CA10E5A5D5100DA90085E2A20295D595DBCA10F938A5E0E5DD85E0A201B5DEE90095DECA10F7B00CA900A20395DDCA10FB20BDA5DEA6DF09F0A4E1F020F09CF0A4A2FEA05A18A5D96905A5D86900B004A2ADA0DE98A4E2F004A5D5A6D685FB86FAA5D9A6D8100538A900E5D985F9A90285E3D8201F1F206A1FC913F0C0B00320AD02C6E3D0EDF0B7C90A9005490F85E160AAA5DDF0FA86DDA5DD38F8E90585DCA900E90085DB604501009981009997020800000101";

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
  // data
  tapesend(data);
  // 2 sec sync
  datasync();
}

void leadsync() {
  for (int n=0; n<100; n++) { tapesendByte('16'); }
  tapesendByte('2A');
}

void datasync(){
  tapesendByte('2F');
  tapesendByte('00');
  tapesendByte('00');
  tapesendByte('04');
  tapesendByte('04');
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
  for (int i=0; i<8;i++){
    (bitRead(c, i)) ? bit1() : bit0();
  }
}

void tapesend(String c){
  for (int i=0; i < c.length(); i+=2){
    tapesendByte((getValue(c[i])<<4) + getValue(c[i+1]));
  }
}

// A high tone is output for 9 cycles and a low tone for 6 cycles.
// each bit is 3 tones
// bit 0 = 3700Hz + 3700Hz + 2400Hz
// bit 0 = 3700Hz + 2400Hz + 2400Hz
void bit0() {
  for (int n=0; n<9; n++) { 
    hightone(); 
  }
  for (int n=0; n<9; n++) { 
    hightone(); 
  }
  for (int n=0; n<6; n++) { 
    lowtone(); 
  }
}

void bit1() {
  for (int n=0; n<9; n++) { 
    hightone(); 
  }
  for (int n=0; n<6; n++) { 
    lowtone(); 
  }
  for (int n=0; n<6; n++) { 
    lowtone(); 
  }
}

void hightone() {
  digitalWrite(out, HIGH);
  delayMicroseconds(interval_3700);
  digitalWrite(out, LOW);
  delayMicroseconds(interval_3700);
}

void lowtone() {
  digitalWrite(out, HIGH);
  delayMicroseconds(interval_2400);
  digitalWrite(out, LOW);
  delayMicroseconds(interval_2400);
}

byte getValue(char c){
  if(c >= '0' && c <= '9')
    return (byte)(c-'0');
  else if (c >= 'a' && c <= 'f')
    return (byte)(c-'a'+10);
  else
    return (byte)(c-'A'+10);
}
