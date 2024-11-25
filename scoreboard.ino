//Import libraries
#include <Adafruit_MCP23X17.h>
#include <DS3231.h>

//Define variables
#define num 16
#define out HIGH
#define in LOW
#define n 2
#define OneA 9
#define OneB 10
#define hr A0
#define mi A1
#define mo A2
#define dy A3
#define bzz 11
#define light A4

//Define arrays for light bulbs
int p[num] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
int zero[11] = { 0, 1, 2, 6, 9, 10, 11, 12, 13, 14, 15 };
int one[5] = { 2, 3, 4, 5, 6 };
int two[10] = { 1, 2, 4, 6, 7, 9, 10, 11, 14, 15 };
int three[9] = { 0, 1, 2, 4, 6, 8, 9, 10, 15 };
int four[7] = { 0, 1, 4, 8, 13, 14, 15 };
int five[10] = { 0, 2, 4, 6, 8, 9, 10, 13, 14, 15 };
int six[12] = { 0, 2, 4, 6, 8, 9, 10, 11, 12, 13, 14, 15 };
int seven[6] = { 0, 1, 6, 9, 14, 15 };
int eight[13] = { 0, 1, 2, 4, 6, 8, 9, 10, 11, 12, 13, 14, 15 };
int nine[11] = { 0, 1, 2, 4, 6, 8, 9, 10, 13, 14, 15 };
int addr[7] = { 25, 21, 23, 26, 24, 20, 22 };

int zeroN[5] = {3,4,5,7,8};
int oneN[11] = {0,1,7,8,9,10,11,12,13,14,15};
int twoN[6] = {0,3,5,8,12,13};
int threeN[7] = {3,5,7,11,12,13,14};
int fourN[9] = {2,3,5,6,7,9,10,11,12};
int fiveN[6] = {1,3,5,7,11,12};
int sixN[4] = {1,3,5,7};
int sevenN[10] = {2,3,4,5,7,8,10,11,12,13};
int eightN[3] = {3,5,7};
int nineN[5] = {3,5,7,11,12};

bool AM;

//Initialize objects for different digits
Adafruit_MCP23X17 hrs;
Adafruit_MCP23X17 minA;
Adafruit_MCP23X17 minB;
Adafruit_MCP23X17 monA;
Adafruit_MCP23X17 monB;
Adafruit_MCP23X17 dayA;
Adafruit_MCP23X17 dayB;

DS3231 rtc(SDA, SCL);
Time t;
Time tmp;

void setup() {
  Serial.begin(9600);

  pinMode(hr,INPUT);
  pinMode(mi,INPUT);
  pinMode(mo,INPUT);
  pinMode(dy,INPUT);
  //while (!Serial);
  pinMode(OneA, OUTPUT);
  pinMode(OneB, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(bzz, OUTPUT);
  pinMode(light, OUTPUT);
  rtc.begin();
  /*
  for(int i = 0; i < num; i++){
    p[i] = i;
  }
*/
  //Initialize digit lights
  bool tmp = hrs.begin_I2C(0x27);
  tmp = minA.begin_I2C(0x21);
  tmp = minB.begin_I2C(0x23);
  tmp = monA.begin_I2C(0x20);
  tmp = monB.begin_I2C(0x22);
  tmp = dayA.begin_I2C(0x24);
  tmp = dayB.begin_I2C(0x25);
  for (int i = 0; i < num; i++) {
    hrs.pinMode(p[i], OUTPUT);
    minA.pinMode(p[i], OUTPUT);
    minB.pinMode(p[i], OUTPUT);
    monA.pinMode(p[i], OUTPUT);
    monB.pinMode(p[i], OUTPUT);
    dayA.pinMode(p[i], OUTPUT);
    dayB.pinMode(p[i], OUTPUT);
  }
  for (int i = 0; i < 16; i++) {
    hrs.digitalWrite(p[i], in);
    minA.digitalWrite(p[i], in);
    minB.digitalWrite(p[i], in);
    monA.digitalWrite(p[i], in);
    monB.digitalWrite(p[i], in);
    dayA.digitalWrite(p[i], in);
    dayB.digitalWrite(p[i], in);
  }

  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(light,LOW);

  t = rtc.getTime();
  digitalWrite(bzz, HIGH);
  delay(1000);
  digitalWrite(bzz, LOW);
}

void loop() {
  digitalWrite(light,HIGH);
  //digitalWrite(OneA, HIGH);
  //digitalWrite(OneB, HIGH);
  /*
  mcp.digitalWrite(p[2], in);
  mcp.digitalWrite(p[0], out);
  mcp.digitalWrite(p[0], in);
  mcp.digitalWrite(p[1], out);
  mcp.digitalWrite(p[1], in);
  mcp.digitalWrite(p[2], out);
*/
  tmp = rtc.getTime();
  // if(t.min == 0 && t.sec == 0){
  //   digitalWrite(bzz,HIGH);
  //   delay(500);
  //   digitalWrite(bzz,LOW);
  // }
  if(t.hour > 11){ // this line had > 12 before, this meant that it would have 2 be 1 o clock for the AM PM to change
    AM = false;
    t.hour= t.hour -12;
    //Serial.println(t.hour);
  }else{
    AM = true;
  }
  if(t.hour == 0){
    t.hour = 12;
  }
  if(t.hour == 12 || t.hour == 11 || t.hour == 10){
    digitalWrite(OneA, HIGH);
    digitalWrite(OneB, HIGH);
  }else{
    digitalWrite(OneA, LOW);
    digitalWrite(OneB, LOW);
  }
  if(AM == false){
    digitalWrite(8,HIGH);
    digitalWrite(7,LOW);
  }else{
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
  }
  displayDigit(t.hour%10, hrs);
  displayDigit((t.min)/10, minA);
  displayDigit((t.min)%10, minB);
  displayDigit((t.date)/10, dayA);
  displayDigit((t.date)%10, dayB);
  displayDigit((t.mon)/10, monA);
  displayDigit((t.mon)%10, monB);

  // digitalWrite(3,HIGH);
  // digitalWrite(4,HIGH);
  // digitalWrite(5,HIGH);
  // digitalWrite(6,HIGH);

  if ((t.mon > 3 && t.mon < 6) || (t.mon == 3 && t.date >= 21) || (t.mon == 6 && t.date < 22)){
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }

  if ((t.mon > 6 && t.mon < 9) || (t.mon == 6 && t.date >= 22) || (t.mon == 9 && t.date < 21)){
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }

  if ((t.mon > 9 && t.mon < 12) || (t.mon == 9 && t.date >= 21) || (t.mon == 12 && t.date < 22)){
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }

  if ((t.mon == 1 || t.mon == 2) || (t.mon == 12 && t.date >= 22) || (t.mon == 3 && t.date < 21)){
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }
  // for(int i =0; i < 16; i++){
  //   mcp.digitalWrite(p[i], out);
  //   delay(1500);
  //   mcp.digitalWrite(p[i], in);
  // }
  // for (int j = 0; j < 10; j++){
  //   displayDigit(j, hrs);
  //   displayDigit(j, minA);
  //   displayDigit(j, minB);
  //   displayDigit(j, monA);
  //   displayDigit(j, monB);
  //   displayDigit(j, dayA);
  //   displayDigit(j, dayB);
  //   delay(500);
  //   for(int i =0; i < 16; i++){
  //     hrs.digitalWrite(p[i], in);
  //     minA.digitalWrite(p[i], in);
  //     minB.digitalWrite(p[i], in);
  //     monA.digitalWrite(p[i], in);
  //     monB.digitalWrite(p[i], in);
  //     dayA.digitalWrite(p[i], in);
  //     dayB.digitalWrite(p[i], in);
  //   }
  //   delay(500);
  // }

  // for (int i = 0; i < 10; i++)
  // {
  //   displayDigit(i);
  //   delay(500);
  //   for(int i =0; i< 16; i++){
  //     mcp.digitalWrite(p[i], in);
  //   }
  //   delay(500);
  // }
  while(t.min == tmp.min){
    tmp = rtc.getTime();
    if(digitalRead(hr) == 0){
      if(tmp.hour == 23){
        tmp.hour = 0;
      }
      else{
        tmp.hour = tmp.hour + 1;
      }
      rtc.setTime(tmp.hour, tmp.min, tmp.sec);
      tmp = rtc.getTime();
      delay(500);
      break;
    }

    if(digitalRead(mi) == 0){
      if(tmp.min == 59){
        tmp.min = 0;
        tmp.sec = 0;
      }
      else{
        tmp.min = tmp.min + 1;
        tmp.sec = 0;
      }
      rtc.setTime(tmp.hour, tmp.min, tmp.sec);
      tmp = rtc.getTime();
      delay(500);
      break;
    }

    if(digitalRead(mo) == 0){
      if(tmp.mon == 12){
        tmp.mon = 1;
      }
      else{
        tmp.mon = tmp.mon + 1;
      }
      rtc.setDate(tmp.date, tmp.mon, tmp.year);
      tmp = rtc.getTime();
      delay(500);
      break;
    }

    if(digitalRead(dy) == 0){
      tmp.date += 1;
      if (tmp.date > 31){
        tmp.date = 1;
      }
      if(tmp.mon == 2 && tmp.date > 29){
        tmp.date = 1;
      }
      if((tmp.mon == 4 || tmp.mon == 6 || tmp.mon == 9 || tmp.mon == 11) && tmp.date > 30){
        tmp.date = 1;
      }
      rtc.setDate(tmp.date, tmp.mon, tmp.year);
      tmp = rtc.getTime();
      delay(500);
      break;
    }
  }
  t = rtc.getTime();
}


/*
void displayBinary(byte numToShow)
{
  for (int i = 0; i < num; i++)
  {
    Serial.println(bitRead(numToShow,i));
    if (bitRead(numToShow, i) == 1)
    {
      mcp.digitalWrite(ledPin[i], HIGH);
    }
    else
    {
      mcp.digitalWrite(ledPin[i], LOW);
    }
  }
}
*/

int displayDigit(int digit, Adafruit_MCP23X17 mcp) {
  if (digit == 0) {
    for (int i = 0; i < 11; i++) {
      mcp.digitalWrite(p[zero[i]], out);
    }

    for (int i = 0; i < 5; i++) {
      mcp.digitalWrite(p[zeroN[i]], in);
    }
    return 0;
  }
  if (digit == 1) {
    for (int i = 0; i < 5; i++) {
      mcp.digitalWrite(p[one[i]], out);
    }
    for (int i = 0; i < 11; i++) {
      mcp.digitalWrite(p[oneN[i]], in);
    }
    return 1;
  }
  if (digit == 2) {
    for (int i = 0; i < 10; i++) {
      mcp.digitalWrite(p[two[i]], out);
    }

    for (int i = 0; i < 6; i++) {
      mcp.digitalWrite(p[twoN[i]], in);
    }
    return 2;
  }
  if (digit == 3) {
    for (int i = 0; i < 9; i++) {
      mcp.digitalWrite(p[three[i]], out);
    }

    for (int i = 0; i < 7; i++) {
      mcp.digitalWrite(p[threeN[i]], in);
    }
    return 3;
  }
  if (digit == 4) {
    for (int i = 0; i < 7; i++) {
      mcp.digitalWrite(p[four[i]], out);
    }

    for (int i = 0; i < 9; i++) {
      mcp.digitalWrite(p[fourN[i]], in);
    }
    return 4;
  }
  if (digit == 5) {
    for (int i = 0; i < 10; i++) {
      mcp.digitalWrite(p[five[i]], out);
    }

    for (int i = 0; i < 6; i++) {
      mcp.digitalWrite(p[fiveN[i]], in);
    }
    return 5;
  }
  if (digit == 6) {
    for (int i = 0; i < 12; i++) {
      mcp.digitalWrite(p[six[i]], out);
    }

    for (int i = 0; i < 4; i++) {
      mcp.digitalWrite(p[sixN[i]], in);
    }
    return 6;
  }
  if (digit == 7) {
    for (int i = 0; i < 6; i++) {
      mcp.digitalWrite(p[seven[i]], out);
    }

    for (int i = 0; i < 10; i++) {
      mcp.digitalWrite(p[sevenN[i]], in);
    }
    return 7;
  }
  if (digit == 8) {
    for (int i = 0; i < 13; i++) {
      mcp.digitalWrite(p[eight[i]], out);
    }
    for (int i = 0; i < 3; i++) {
      mcp.digitalWrite(p[eightN[i]], in);
    }
    return 8;
  }
  if (digit == 9) {
    for (int i = 0; i < 11; i++) {
      mcp.digitalWrite(p[nine[i]], out);
    }

    for (int i = 0; i < 5; i++) {
      mcp.digitalWrite(p[nineN[i]], in);
    }

    return 9;
  }
}
