#include <SoftwareSerial.h>
#define siren 8
#define trigPin 9
#define echoPin 10
SoftwareSerial mySerial(0, 1);

char msg;
char call;
long duration;
int distance;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(siren,OUTPUT);  
mySerial.begin(9600);
Serial.begin(9600); 

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.println(distance);
  delay(1000);
  if(distance<=15)
  {
  SendMessage();
  }
  else
  {
    digitalWrite(siren,HIGH);
    MakeCall();
    while(!mySerial.available());
    digitalWrite(siren,LOW);
    while(distance>15);
  }

}
void SendMessage()
{
Serial.println("Sending Message");
mySerial.println("AT+CMGF=1");
delay(1000);
mySerial.println("AT+CMGS=\"+919896086536\"\r");
delay(1000);
mySerial.println("shutter is closed and device is activated");
delay(100);
}
void MakeCall()
{
mySerial.println("ATD++919896086536;");
Serial.println("Calling ");
delay(5000);
}
