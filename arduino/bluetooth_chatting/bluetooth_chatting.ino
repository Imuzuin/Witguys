#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출
 
int blueTx=2;   //Tx (보내는핀 설정)
int blueRx=3;   //Rx (받는핀 설정)

SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언
String receivedString = "";
String sentString = "";
 
void setup() {
  Serial.begin(9600);   //시리얼모니터 
  mySerial.begin(9600); //블루투스 시리얼 개방
}
 
void loop() {
  while(mySerial.available()) {
    char myChar = (char)mySerial.read();
    receivedString += myChar;
    
    delay(5);           //수신 문자열 끊김 방지 9600 기준 1이 필요하다고 함
  }
  
  if(!receivedString.equals("")) {
    Serial.println("input value: " + receivedString);
    receivedString = "";
  }

  while (Serial.available()) {
    char myInput = (char)Serial.read();
    Serial.println(myInput);
    
    sentString += myInput;
    
    mySerial.write(myInput);
  }

  if(!sentString.equals("")) {
    Serial.println("output value: " + sentString);
    sentString = "";
  }
}
