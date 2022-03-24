#include <RH_RF95.h>
#include <Wire.h>//acc
#include <stdlib.h>
#include <string.h>

int ADXL345 = 0x53;//acc
float X_out, Y_out, Z_out;//acc
char id[] = "0001S";

//radio setup
#define RF95_CS 10 
#define RF95_INT 2
#define RF95_RST 9
#define RF95_FREQ 868.0//915.0
RH_RF95 rf95(RF95_CS, RF95_INT);
//uint8_t data[RH_RF95_MAX_MESSAGE_LEN];

int led = 13; // led pin

//gps setup
double lat = 42.199457; //latitude
double lon = 25.324184; //longitude

float maximum_X = 0;
float maximum_Y = 0;
float maximum_Z = 0;

int counter_X = 0;
int counter_Y = 0;
int counter_Z = 0;

//default axis values
float X_def = 0.01;
float Y_def = -0.1;
float Z_def = 0.96;

//data packaging
char result[64];
char res_X[6];
char res_Y[6];
char res_Z[6];
char res_lat[10];
char res_lon[10];
char res_mag[4];

void setup() {
  Wire.begin();//acc
  Wire.beginTransmission(ADXL345);//acc
  Wire.write(0x2D);//acc
  Wire.write(8);//acc
  Wire.endTransmission();//acc
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

  Wire.beginTransmission(ADXL345); //acc
  Wire.write(0x32); //acc
  Wire.endTransmission(false); //acc
  Wire.requestFrom(ADXL345, 6, true); //acc

  X_out = (Wire.read()| Wire.read() << 8); //acc
  X_out = X_out/256; //acc
  detect(X_out, X_def, (char*)"Horizontal", counter_X, maximum_X, 1);
  
  Y_out = (Wire.read()| Wire.read() << 8);//acc
  Y_out = Y_out/256; //acc
  //detect(Y_out, Y_def, (char*)"Horizontal", counter_Y, maximum_Y, 2);

  Z_out = (Wire.read()| Wire.read() << 8); //acc
  Z_out = Z_out/256; //acc
  detect(Z_out, Z_def, (char*)"Vertical", counter_Z, maximum_Z, 3);
   
  delay(1000);
}

void Transmit(float mag, char* type){ //sending data over LoRa
  //formating
  strcat(result, id);
  strcat(result, "/");
  dtostrf(mag, 2, 1, res_mag);//
  strcat(result, res_mag);
  strcat(result, "/");
  strcat(result, type);
  strcat(result, "/");
  dtostrf(lat, 6, 6, res_lat);
  strcat(result, res_lat);
  strcat(result, "/");
  dtostrf(lon, 6, 6, res_lon);
  strcat(result, res_lon);

  rf95.send((uint8_t*)result,sizeof(result));
  rf95.waitPacketSent();
  
  Serial.write((char*)result);
  Serial.write("\n");
  
  memset(result, 0, 64);
}

void detect(float axis, float axis_def, char* type, int counter, float maximum, int variant){ //detecting earthquakes
  float num;
  if(axis > axis_def){
    num = axis-axis_def;
  }
  else{
    num = axis_def-axis;
  }
  float res = magnitude(num);
  if(res > maximum){
    maximum = res;
    Transmit(maximum, type);
    counter = 0;
  }
  else if(res == 0 && counter <3 && maximum!=0){
    counter = counter + 1;
  }
  else if(counter == 3 && maximum!=0){
    counter = 0;
    maximum = 0;
    Transmit(maximum, type);
  }

  switch(variant){ //checking variant
    case 1:
      maximum_X = maximum;
      counter_X = counter;
      break;
    case 2:
      maximum_Y = maximum;
      counter_Y = counter;
      break;
    case 3:
      maximum_Z = maximum;
      counter_Z = counter;
      break;
  }
}

float magnitude(float dif){ //assigning magnitude
  int dif_int = dif * 100;
  switch(dif_int){
    case 0 ... 4:
      return 0;
      break;
    case 5 ... 6:
      return 4;
      break;
    case 7 ... 9:
      return 4.5;
      break;
    case 10 ... 14:
      return 5;
      break;
    case 15 ... 19:
      return 5.5;
      break;
    case 20 ... 24:
      return 6;
      break;
    case 25 ... 29:
      return 6.5;
      break;
    case 30 ... 44:
      return 7;
      break;
    case 45 ... 59:
      return 7.5;
      break;
    case 60 ... 69:
      return 8;
      break;
    case 70 ... 79:
      return 8.5;
      break;
    case 80 ... 89:
      return 9;
      break;
    case 90 ... 99:
      return 9.5;
      break;
    case 100 ... 1000:
      return 10;
      break;
    default:
      return 0;
      break;
  }
}
