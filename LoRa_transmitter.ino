 
#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_CS 10
#define RFM95_RST 7
#define RFM95_INT 2
 
#define RF95_FREQ 915.0
 
RH_RF95 rf95(RFM95_CS, RFM95_INT);

int16_t water_level = 0;
 
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  Serial.println("transmitter side!!!!");
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
  water_level=analogRead(A0);
  Serial.println("Sending data to receiver");
  
  String water_level_data =String(water_level); 
  
  Serial.print("Sending "); 
  Serial.println(water_level_data); 
  delay(10);
  rf95.send((uint8_t*)(water_level_data.c_str()), water_level_data.length()+1);
 
  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  Serial.println("Waiting for reply of receiver....."); 
  delay(10);
  if (rf95.waitAvailableTimeout(1000))
  {  
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply,there is no listener around?");
  }
  delay(10000); 
}
