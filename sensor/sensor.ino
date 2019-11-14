#include <SoftwareSerial.h>
#include <DHT11.h>
#include <ArduinoJson.h>

DHT11 dht11(A0);

int flamePin[] = {A2, A3, A4, A5};
const int smokePin = A1;
float humi, temp; 
int flame[] = {0, 0, 0, 0};
int smoke;
int data = 0;

void sensors();

void setup() {
  Serial.begin(9600);
  for(int i = 0; i<4; i++){
    pinMode(flamePin[i], INPUT);
  }
}

void loop() {
  sensors();
  delay(5000);
}

void sensors(){
  dht11.read(humi, temp);
  for(int i = 0; i<4; i++){
    flame[i] = analogRead(flamePin[i]);
  }
  smoke = analogRead(smokePin);
  DynamicJsonDocument doc(1024); // doc 라는 동적 json문서 생성
  doc["Temperature"] = temp;
  doc["Humidity"] = humi;
  doc["Flame1"] = flame[0];
  doc["Flame2"] = flame[1];
  doc["Flame3"] = flame[2];
  doc["Flame4"] = flame[3];
  doc["Smoke"] = smoke;
  doc["motorSignal"] = data;
  serializeJson(doc, Serial);
  Serial.println();
}
