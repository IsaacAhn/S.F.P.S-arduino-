#include <SoftwareSerial.h>
#include "HCPCA9685.h"
#define  I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);

int k[10];
int x = 0, y = 0, z = 0;
int edgeTop[] = { 90, 90, 90, 28, 55, 65, 0, 35, 50 };
int edgeBottom[] = { 90, 65, 50, 72, 60, 50, 55, 53, 45 };
int raspData;
int temp;

//소화기 모터 각도 계산
void angle(int j);
// 소화기 모터 회전
void turnEdge(int a);
//라즈베리파이로부터 들어온 데이터 수신
void recieveData();

void setup() {
  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(true);
}

void loop() {
  recieveData();
}

//소화기 모터 각도 계산
void angle(int j) {
  for(int i=0; i<j; i++){
    if(i == 0) {
      x = 2;
      y = 6;
      z = 8;
    }else if(i == 3){
      x = 5;
      y = 3;
      z--;
    }else if(i == 6){
      x = 8;
      y = 0;
      z--;
    }else {
      x--;
      y++;
      z--;
    }
  }
}

// 소화기 모터 회전
void turnEdge(int a) {
  k[0] = map(edgeTop[a-1], 0, 90, 310, 105);
  k[1] = map(edgeBottom[a-1], 0, 90, 285, 85);
  k[2] = map(edgeTop[x], 0, 90, 85, 290);
  k[3] = map(edgeBottom[x], 0, 90, 115, 320);
  k[4] = map(edgeTop[y], 0, 90, 80, 290);
  k[5] = map(edgeBottom[y], 0, 90, 110, 315);
  k[6] = map(edgeTop[z], 0, 90, 385, 185);
  k[7] = map(edgeBottom[z], 0, 90, 385, 170);

  for (int j = 0; j < 8; j++) {
    HCPCA9685.Servo(j, k[j]);
  }
}

//라즈베리파이로부터 들어온 데이터 수신
void recieveData(){
  while ( Serial.available() > 0 ){
    raspData = Serial.parseInt();
    Serial.println(raspData);
    angle(raspData);
    HCPCA9685.Sleep(false);
    turnEdge(raspData);
  }
}
