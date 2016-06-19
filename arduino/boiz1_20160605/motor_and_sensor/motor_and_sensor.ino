#include <AFMotor.h>

AF_DCMotor motor1(3);
AF_DCMotor motor2(4);

int trigPin = 2;
int echoPin = 3;
 
//시리얼 속도설정, trigPin을 출력, echoPin을 입력으로 설정
void setup(){
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   
  motor1.run(RELEASE);
  motor2.run(RELEASE);

  motor1.setSpeed(255);  
  motor2.setSpeed(255);  
}
 
//초음파를 보낸다. 다 보내면 echo가 HIGH(신호받기) 상태로 대기
void loop(){
  float duration, distance;
  
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  if(digitalRead(echo) == LOW) {
    duration = pulseIn(echoPin, HIGH);
    distance = ((float)(340 * duration) / 10000) / 2;  
  } else {
    pinMode(echo, OUTPUT);
    digitalWrite(echo, LOW);
    pinMode(echo, INPUT);

    distance = 11;
  }
 
  Serial.println(distance); //센서값을 시리얼 모니터에 출력
 
  if (distance > 10) {  
      motor1.run(FORWARD);
      motor2.run(FORWARD);
  } else {
      //motor1.run(RELEASE);    
      //motor2.run(RELEASE);

      motor1.run(BACKWARD);
      motor2.run(FORWARD);
  }
}
