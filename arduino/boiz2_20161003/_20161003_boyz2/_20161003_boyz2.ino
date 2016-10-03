#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출

//DC모터 설정
int motorRightBackward = 3;
int motorRightForward = 4;
int motorLeftForward = 5;
int motorLeftBackward = 6;

//LED 설정
int numLEDs = 3;
int ledPinArray[] = {7, 13, 12};
int rgbStopArray[] = {155, 0, 0}; //red
int rgbAutoModeArray[] = {0, 155, 0}; //green
int rgbForwardArray[] = {255, 051, 204}; //pink
int rgbBackwardArray[] = {255, 255, 255}; //brown
int rgbLeftArray[] = {155, 155, 53};//yellow
int rgbRightArray[] = {53, 53, 155};//blue

//블루투스 설정
int blueTx=10;   //Tx (보내는핀 설정)
int blueRx=11;   //Rx (받는핀 설정)

//시리얼 통신을 위한 객체선언
SoftwareSerial btSerial(blueTx, blueRx);  
String receivedString = "";

//초음파센서 설정
int trigPin = 9;
int echoPin = 8;

//자동 수동모드 플래그
bool isAutoDrive = false;

void stopDrive() {
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);

  //LED 설정
  setLedColor(rgbStopArray);
}

void setLedColor(int arr[]) {
  for (int index = 0; index < numLEDs; index++) {
    analogWrite(ledPinArray[index], arr[index]);  
  }  
}

void drive(int p1, int p2, int p3, int p4, int arr[]) {
  //LED 설정
  setLedColor(arr);
  
  digitalWrite(motorRightForward, p1);
  digitalWrite(motorRightBackward, p2);
  digitalWrite(motorLeftForward, p3);
  digitalWrite(motorLeftBackward, p4);  
  delay(105);

  //LED 대기상태로 변환
  setLedColor(rgbStopArray);
}
 
//시리얼 속도설정, trigPin을 출력, echoPin을 입력으로 설정
void setup(){
  Serial.begin(9600);
  btSerial.begin(9600); //블루투스 시리얼 개방

  //초음파센서 핀설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  //DC모터 핀설정
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);
  pinMode(motorLeftForward, OUTPUT);  
  pinMode(motorLeftBackward, OUTPUT);

  stopDrive();
}
 
//초음파를 보낸다. 다 보내면 echo가 HIGH(신호받기) 상태로 대기
void loop() {
    float duration, distance;
    char inputCommand;

    if (btSerial.available()) {  
        inputCommand = (char)btSerial.read();
        Serial.println("input command : " + inputCommand);

        if (inputCommand == 'm') {
            isAutoDrive = false;  
        } else if (inputCommand == 'a') {
            isAutoDrive = true;
            setLedColor(rgbAutoModeArray);
        }
    }

    //========================================
    // 아래는 수동제어 모드
    //========================================  
    if (isAutoDrive == false) {
        if (inputCommand == 'f') {
            drive(HIGH, LOW, HIGH, LOW, rgbForwardArray);
        } else if(inputCommand == 'b') {
            drive(LOW, HIGH, LOW, HIGH, rgbBackwardArray);
        } else if(inputCommand == 'r') {
            drive(LOW, HIGH, HIGH, LOW, rgbRightArray);
        } else if(inputCommand == 'l') {
            drive(HIGH, LOW, LOW, HIGH, rgbLeftArray);
        } else {
            stopDrive();
        }
    } 
    //========================================
    // 아래 코드는 자동제어용 코드
    //========================================
    else { 
        if (isAutoDrive == true) {
            digitalWrite(trigPin, HIGH);
            delay(10);
            digitalWrite(trigPin, LOW);
        
            if (digitalRead(echoPin) == LOW) {
                duration = pulseIn(echoPin, HIGH);
                distance = ((float)(340 * duration) / 10000) / 2;  
            } else {
                pinMode(echoPin, OUTPUT);
                digitalWrite(echoPin, LOW);
                pinMode(echoPin, INPUT);

                //임의설정값
                distance = 11;
            }
         
            Serial.println(distance); //센서값을 시리얼 모니터에 출력
         
            if (distance > 10) {  
                digitalWrite(motorRightForward, HIGH);
                digitalWrite(motorRightBackward, LOW);
                digitalWrite(motorLeftForward, HIGH);
                digitalWrite(motorLeftBackward, LOW);
            } else {
                digitalWrite(motorRightForward, LOW);
                digitalWrite(motorRightBackward, HIGH);
                digitalWrite(motorLeftForward, HIGH);
                digitalWrite(motorLeftBackward, LOW);
            }    
        }    
    }
}
