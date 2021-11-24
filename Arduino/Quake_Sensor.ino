int led = 13; // led pin
int vibSensor = 9; // vibration sensor pin
long sum = 0;
int counter = 30;
float magn = 0; //magnitude
float magnPrev = -1; //previous magnitude
double lat = 42.199457; //latitude
double lon = 25.324184; //longitude

void setup() {
  pinMode(led, OUTPUT);
  pinMode(vibSensor, INPUT);
  Serial.begin(9600); //initialising serial communication
}

void loop() {
  
  int measurement = vibration(); //measuring seismic activity
  if(counter != 0){ //averaging every 30 measurements
    sum = sum + measurement;
    counter = counter - 1;
  }
  
  else{
    magn = magnitude(sum / 30);
    sum = 0;
    counter = 30;
  }
  
  delay(50);
  
  if(magn != magnPrev){
    String line = JoinOutput(magn, lat, lon);
    Serial.println(line); //sending data
    magnPrev = magn;
  }
  
  if(measurement > 50){ //signaling
    digitalWrite(led, HIGH);
  }
  
  else{
    digitalWrite(led, LOW);
  }

}

int vibration(){ //measuring wave strength
  int measurement = pulseIn(vibSensor,HIGH);
  return measurement;
}

float magnitude(long average){ //assigning magnitude
  switch(average){
    case 100 ... 499:
      return 4;
      break;
    case 500 ... 999:
      return 4.5;
      break;
    case 1000 ... 4999:
      return 5;
      break;
    case 5000 ... 9999:
      return 5.5;
      break;
    case 10000 ... 49999:
      return 6;
      break;
    case 50000 ... 99999:
      return 6.5;
      break;
    case 100000 ... 499999:
      return 7;
      break;
    case 500000 ... 999999:
      return 7.5;
      break;
    case 1000000 ... 10000000:
      return 8;
      break;
    default:
      return 0;
      break;
  }

}

String JoinOutput(float magnitudes, double latitude, double lonitude){ //packaging output data
  String output = String(magnitudes);
  output += "-";
  output += String(latitude,7);
  output += "-";
  output += String(lonitude,7);
  return output;
}
