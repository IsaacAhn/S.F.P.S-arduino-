#include "HCPCA9685.h"
#define  I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);
int k[10];
int x = 0, y = 0, z = 0;
int centerTop[] = { 100, 80, 70, 80, 70, 75, 135, 130, 135 };
int centerBottom[] = { 0, 0, 45, 90, 135, 180, 225, 270, 315 };
int edgeTop[] = { 67, 75, 60, 57, 50, 55, 59, 74, 90 };
int edgeBottom[] = { 55, 90, 90, 75, 58, 40, 19, 29, 90 };


// 중앙 카메라 회전 (0~360도를 넣으면 약 45도의 각도로 회전)
void turnCenter(int upValue, int downValue);
// 소화기 모터 회전
void turnEdge(int i);
// 라즈베리파이로부터 데이터 수신
void recieveData();

void setup()
{
  Serial.begin(9600);
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
}


void loop()
{
  if (Serial.available() > 0) {
    for (int i = 0; i<9; i++) {
      Serial.print("I seeing: ");
      Serial.println(i, DEC);
      turnCenter(centerTop[i], centerBottom[i]);
      turnEdge(i);
      delay(2000);
    }
//    recieveData();
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

// 소화기 모터 회전
void turnEdge(int i) {
  k[0] = map(edgeTop[i], 0, 90, 295, 80);
  k[1] = map(edgeBottom[i], 0, 90, 315, 110);
  k[2] = map(edgeTop[x], 0, 90, 115, 320);
  k[3] = map(edgeBottom[x], 0, 90, 170, 385);
  k[4] = map(edgeTop[y], 0, 90, 40, 255);
  k[5] = map(edgeBottom[y], 0, 90, 90, 305);
  k[6] = map(edgeTop[z], 0, 90, 385, 175);
  k[7] = map(edgeBottom[z], 0, 90, 390, 185);

  for (int j = 0; j < 8; j++) {
    HCPCA9685.Servo(j, k[j]);
  }
  if (i == 0) {
    x = 1;
    y = 5;
    z = 5;
  }
  else {
    x--;
    y--;
    z++;
  }
  if (x == 0) {
    x = 8;
  }
  if (y == 0) {
    y = 8;
  }
  if (z == 9) {
    z = 1;
  }
}

// 라즈베리파이로부터 데이터 수신
void recieveData() {
  char sd = Serial.read();
  Serial.println(sd);

  if (sd == '1') {
    HCPCA9685.Sleep(true);
  }
  else {
    HCPCA9685.Sleep(false);
  }
}
