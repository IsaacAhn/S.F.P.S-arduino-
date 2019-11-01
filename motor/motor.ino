#include <SoftwareSerial.h>
#include "HCPCA9685.h"
#define  I2CAdd 0x40

SoftwareSerial s(2,3); 
HCPCA9685 HCPCA9685(I2CAdd);

int k[10];
int x = 0, y = 0, z = 0; 
int centerTop[] = { 100, 80, 70, 80, 70, 75, 135, 130, 135 };
int centerBottom[] = { 0, 0, 45, 90, 135, 180, 315, 270, 225 };
int edgeTop[] = { 60, 65, 50, 50, 40, 50, 82, 65, 55 };
int edgeBottom[] = { 50, 85, 85, 65, 50, 30, 45, 20, 5 };
int raspData = 3;
int temp;
int i = 0;
int count = 0;

// 중앙 카메라 회전 (0~360도를 넣으면 약 45도의 각도로 회전)
void turnCenter(int upValue, int downValue);

void angle(int i);
// 소화기 모터 회전
void turnEdge(int a);

void recieveData();

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(true);
}

void loop() {
  recieveData();
  if(raspData==0){
    turnCenter(centerTop[i], centerBottom[i]);
    delay(1000);
    if(count >= 0 && i > 2){
      angle(i);
    }
    if(i == 0){
      s.write(8);
    }
    temp = i;
    i++;
    if(i == 9){
      i = 0;
      count++;
    }
    if(count == 2){
      HCPCA9685.Sleep(true);
      if(i = 2){
        s.write(9);
        i = 0;
        count = 0;
        raspData = 3;
      }
    }
  } else if(raspData == 1){
    HCPCA9685.Sleep(true);
    i = 0;
    count = 0;
    s.write(1);
    raspData = 3;
  } else if(raspData==2){
    turnCenter(centerTop[temp-2], centerBottom[temp-2]);
    turnEdge(temp-2);
    raspData = 3;
  }
}

// 중앙 카메라 회전 (0~360도를 넣으면 약 45도의 각도로 회전)
void turnCenter(int upValue, int downValue) {
  if (downValue >= 0 && downValue <= 180) {
    k[8] = map(upValue, 0, 180, 15, 415);
    k[9] = map(downValue, 0, 180, 10, 420);
  }
  else {
    k[8] = map(upValue, 0, 180, 15, 415);
    k[9] = map(downValue - 180, 0, 180, 10, 420);
  }
  HCPCA9685.Servo(8, k[8]);
  HCPCA9685.Servo(9, k[9]);
}

void angle(int i) {
  if(i == 2) {
    x = 0;
    y = 0;
    z = 0;
  }else if(i == 3) {
    x = 1;
    y = 5;
    z = 5;
  }else if(i == 4) {
    x = 6;
    y--;
    z = 8;
  }else if(i == 7){
    x++;
    y--;
    z = 1;
  }else if(i == 8){
    x = 2;
    y--;
    z--;
  }else {
    x++;
    y--;
    z--;
  }
  
  if (x == 9) {
    x = 5;
  }
  if (y == 0 && x != 0) {
    y = 8;
  }
  if (z == 0 && x != 0) {
    z = 4;
  }
}

// 소화기 모터 회전
void turnEdge(int a) {
  k[0] = map(edgeTop[a], 0, 90, 295, 80);
  k[1] = map(edgeBottom[a], 0, 90, 315, 110);
  k[2] = map(edgeTop[x], 0, 90, 115, 320);
  k[3] = map(edgeBottom[x], 0, 90, 170, 385);
  k[4] = map(edgeTop[y], 0, 90, 40, 255);
  k[5] = map(edgeBottom[y], 0, 90, 90, 305);
  k[6] = map(edgeTop[z], 0, 90, 385, 175);
  k[7] = map(edgeBottom[z], 0, 90, 390, 185);

  for (int j = 0; j < 8; j++) {
    HCPCA9685.Servo(j, k[j]);
  }
}

void recieveData(){
  if ( Serial.available() > 0 ){
    raspData = Serial.parseInt();
    Serial.println(raspData);
    if (raspData == 0){
      HCPCA9685.Sleep(false);
    } else if (raspData == 1) {
      HCPCA9685.Sleep(true);
    } else if (raspData == 2) {
      HCPCA9685.Sleep(false);
    }
  }
}
