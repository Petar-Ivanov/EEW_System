//libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 
#include <math.h> 

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET); //OLED display is defined

#define LED 13  // LED pin

int buzzer = 8; //buzzer pin
bool alertComm = false; //alert system status
double lat = 42.139069; //latitude
double lon = 24.738218; //longitude
float oldStr = 0; //previous comm input
String receivedString = ""; //current comm input
bool timerSwitch = false; 
int timer = 0; 

void setup() {
  Wire.begin();
  Serial.begin(9600); //initialising serial communication
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  
  pinMode(LED, OUTPUT); 
  pinMode(buzzer,OUTPUT);
}

void loop() {
  if(timerSwitch == true){ //seismic timer
    
    if(timer <= 0 )timerSwitch = false;
    
    else{
      delay(1000);
      timer = timer - 1;
      display.clearDisplay();
      displayData(oldStr, timer);
      display.display();
    }
  }
  
    if(Serial.available() > 0){    //detecting serial input
      receivedString = "";
      int switchNum = 1;
      String latSensorStr = ""; 
      String lonSensorStr = ""; 
      
      while(Serial.available() > 0){ //reading serial input  
        char current = char(Serial.read());

        if(current == '-'){  //separating data
          switchNum = switchNum + 1;
          current = char(Serial.read());
        }
        
        //data format: magnitude-latitude-longitude
        switch(switchNum){ //sorting data
          case 1:
            receivedString += current;
            break;
          case 2:
            latSensorStr += current;
            break;
          case 3:
            lonSensorStr += current;
            break;
        }
      }
      //parsing coordinates
      double latSensor = latSensorStr.toDouble(); 
      double lonSensor = lonSensorStr.toDouble();
       
      if(receivedString.toFloat() > 0){ //detecting earthquakes
        
        if(receivedString.toFloat() > oldStr){ //searching for increase in seismic activity
          oldStr = receivedString.toFloat();
        }
       
        alertComm = true; //turning on the alert system
        
        if(timerSwitch == false){
          timer = distance(lat, latSensor, lon, lonSensor)/4;
          timerSwitch = true;
        }
      }
      
      else{
        alertComm = false; // turning off the alert system
        oldStr = 0;
      }
  }

  if(alertComm == true){ //alerting
    alert(true);
    display.clearDisplay();
    displayData(oldStr, timer);
    display.display();
  }
  else if(timerSwitch == false){ 
    alert(false);
    display.clearDisplay();
    display.display();
  }
}
  
void displayData(float magnitude, float timer){ //displaying data
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("EARTHQUAKE WARNING!!!");
  display.setCursor(0,10);
  display.print("Magnitude:  ");
  display.print(magnitude);
  display.setCursor(0,20);
  display.print("Time:       ");
  display.print(timer);
  display.print(" sec.");
  
}

void alert(bool on){ //buzzer alerting
 
  if(on){ 
  digitalWrite(buzzer, HIGH);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  digitalWrite(LED, LOW);
  delay(250);
  digitalWrite(buzzer, HIGH);
  digitalWrite(LED, HIGH);
  delay(500);
  
  if(timer>0) timer = timer - 1; //timer countdown
  else if(timer<0) timer = 0;
  refresh();
  
  digitalWrite(buzzer, LOW);
  digitalWrite(LED, LOW);
  delay(1000);
  
  if(timer>0) timer = timer - 1; //timer countdown
  else if(timer<0) timer = 0;
  refresh();
  
  digitalWrite(buzzer, HIGH);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  digitalWrite(LED, LOW);
  delay(250);
  digitalWrite(buzzer, HIGH);
  digitalWrite(LED, HIGH);
  delay(500);
  
  if(timer>0) timer = timer - 1; //timer countdown
  else if(timer<0) timer = 0;
  refresh();
  
  digitalWrite(buzzer, LOW);
  digitalWrite(LED, LOW);
  delay(1000);
  
  if(timer>0) timer = timer - 1; //timer countdown
  else if(timer<0) timer = 0;
  refresh();
  }
  else{
    
    digitalWrite(buzzer, LOW); //buzzer off
    digitalWrite(LED, LOW);
    
  }
}

static double toRadians(double angleIn10thofaDegree){ //radian conversion
  return (angleIn10thofaDegree * 3.14159265359) / 180;
}

static double distance(double lat1, double lat2, double lon1,double lon2){ //calculating distance between two points
  lon1 = toRadians(lon1);
  lon2 = toRadians(lon2);
  lat1 = toRadians(lat1);
  lat2 = toRadians(lat2);

  // Haversine formula
  double dlon = lon2 - lon1;
  double dlat = lat2 - lat1;
  double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);

  double c = 2 * asin(sqrt(a));
  double r = 6371;
  
  return (c * r);
}

void refresh(){ //refreshing the display
  display.clearDisplay();
  displayData(oldStr, timer);
  display.display();
}
