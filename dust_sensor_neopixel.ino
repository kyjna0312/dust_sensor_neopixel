/*
Create by BananaCoding
WebSite: bananacoding.ac
CEO: 최은희
작성자: 김정준, 김용진
e-mail: youngjin.kim@bananacoding.ac
gitHub: https://github.com/kyjna0312/dust_snesor_neopixel
*/

#include <Adafruit_NeoPixel.h> //네오픽셀 라이브러리

int pin = 8;  //먼지센서

int neoPin = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, neoPin, NEO_GRB + NEO_KHZ800);

unsigned long duration;   //지속 시간
unsigned long starttime;  //시작 시간
unsigned long sampletime_ms = 5000;   //샘플시간 5초 마다 업데이트
unsigned long lowpulseoccupancy = 0;   //Low 신호가 지속된 시간을 초기화
float ratio = 0;  //비율
float concentration = 0;  //입자 농도 0으로 초기화
float pcsPerCF = 0;  //한 입자당 CF를 0으로 초기화
float ugm3 = 0;  //최종 값으로 세제곱미터 당 마이크로 그램(㎍/㎥)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  strip.begin();
  strip.show();
  
  pinMode(pin, INPUT); //미세먼지 센서 입력
  starttime = millis();  //현재 시간 대입  

}

void loop() {
  strip.show();
  duration = pulseIn(pin, LOW); 
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms)  {   //만약 샘플 시간이 5초라면(위에서 정한 샘플 시간(sampletime_ms=5000, starttime=millis()))
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // 정수 백분율
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // 미세먼지 센서 사양 시트 곡선 사용
    pcsPerCF = concentration * 100;  // 입자 농도에 100을 곱하면 입자당 CF값
    ugm3 = pcsPerCF / 13000;  //입자당 CF를 13000으로 나누면 미터세제곱당 마이크로그람의 미세먼지 측정값
    Serial.println(ugm3);
    delay(100);
   if (ugm3 > 0.01 ){   // 만약에 결과값이 0.01보다 크면 미세먼지 값을 출력하라
     // OUTPUT
     lowpulseoccupancy = 0;
      starttime = millis();
    }
    
    if (ugm3 > 0.01 && ugm3 <= 30) {   //만약 미세먼지 값이 0.01 보다 크고 30이랑 같거나 작으면 아래를 출력
      // OUTPUT
      Serial.println("Good");
      strip.setPixelColor(0, 0,0,255);
    }    
   else if (ugm3 > 30 && ugm3 <= 80) {  //만약 미세먼지 값이 30보다 크고 80이랑 같거나 작으면 아래를 출력
      // OUTPUT
      Serial.println("SoSo");
      strip.setPixelColor(0, 255,228,0);
   }
   else if (ugm3 > 80 && ugm3 <= 150) {  //만약 미세먼지 값이 80보다 크고 150이랑 같거나 작으면 아래를 출력
    // OUTPUT
    Serial.println("Bad");
    strip.setPixelColor(0, 255,0,221);
   }
   else if (ugm3 > 150) {  //만약 미세먼지 값이 150 보다 크면 아래를 출력
     // OUTPUT
     Serial.println("Very Bad");
     strip.setPixelColor(0, 255,0,0);
   }  
  }
}

