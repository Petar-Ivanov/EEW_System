#include <RH_RF95.h>

char id[] = "0001G";

//radio setup
#define RF95_CS 10
#define RF95_INT 2
#define RF95_RST 9
#define RF95_FREQ 868.0//915.0
RH_RF95 rf95(RF95_CS, RF95_INT);
uint8_t data[RH_RF95_MAX_MESSAGE_LEN];
char package[64] = "";

void setup() {
  //radio reset
  pinMode(RF95_RST, OUTPUT);
  digitalWrite(RF95_RST, HIGH);
  delay(10);
  digitalWrite(RF95_RST, LOW);
  delay(10);
  digitalWrite(RF95_RST, HIGH);
  delay(10);
  
  rf95.init(); //default settings
  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);
  
  Serial.begin(9600); //initialising serial communication
}

void loop() {
  if(rf95.available()){
    uint8_t len = sizeof(data);//
    rf95.recv(data,&len);//
    
    strcat(package,id);
    strcat(package,"/");
    strcat(package,(char*)data);
    
    Serial.println(package); //recasting received package
    memset(package, 0, 64);
    delay(1000);
  }

}
