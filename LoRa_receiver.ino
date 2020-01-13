
#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 7
#define RFM95_INT 2

#define RF95_FREQ 915.0

#define red_light 3
#define green_light 4
#define yellow_light 5
#define buzzer 6

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{   
  pinMode(red_light,OUTPUT);
  pinMode(green_light,OUTPUT);
  pinMode(yellow_light,OUTPUT);
  pinMode(buzzer,OUTPUT);

  digitalWrite(red_light,LOW);
  digitalWrite(green_light,LOW);
  digitalWrite(yellow_light,LOW);
  digitalWrite(buzzer,LOW);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("reciever side");
  
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa init failed");
    while (1);
  }
  Serial.println("LoRa init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); 
  Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);
}

void loop()
{
  if (rf95.available())
  {   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      String data_received=(char*)buf;
      Serial.println(data_received);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      
      uint8_t data[] = "data transmission successful";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      if(data_received<="100")
      {
        digitalWrite(red_light,LOW);
        digitalWrite(green_light,HIGH);
        digitalWrite(yellow_light,LOW);
        for(int i=0;i<100;i++)
        {
          digitalWrite(buzzer,HIGH);
          delay(1000);
          digitalWrite(buzzer,LOW);
        }
      }
      else if(data_received>="101" && data_received<="200")
      {
        digitalWrite(red_light,LOW);
        digitalWrite(green_light,LOW);
        digitalWrite(yellow_light,HIGH);
         for(int i=0;i<100;i++)
        {
          digitalWrite(buzzer,HIGH);
          delay(100);
          digitalWrite(buzzer,LOW);
        }
      }
      else
      {
        digitalWrite(red_light,HIGH);
        digitalWrite(green_light,LOW);
        digitalWrite(yellow_light,LOW);
         for(int i=0;i<100;i++)
        {
          digitalWrite(buzzer,HIGH);
          delay(10);
          digitalWrite(buzzer,LOW);
        }
      }
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
