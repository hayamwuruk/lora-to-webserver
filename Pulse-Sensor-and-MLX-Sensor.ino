//this code functon is for mlx90614 & pulse sensor
//setup requirement library
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

const int PulseWire = 0;
int Threshold = 550;
// init sensor
PulseSensorPlayground pulseSensor;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("ID"); 
  pulseSensor.analogInput(PulseWire); 
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
  mlx.begin();  
  delay(1000);
}

void pulse(){
  int myBPM = pulseSensor.getBeatsPerMinute(); //get data from pulse sensor by define with myBPM
      if (pulseSensor.sawStartOfBeat()) {
        Serial.print("BPM : ");
        Serial.println(myBPM);
     
      }
      delay(200);
  }
void suhu (){
  float t = mlx.readObjectTempC(); // get data from mlx sensor by define with t
  Serial.print("*C\tObject = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("*C");
   if (t >=37){
    Serial.println("Object conditon above 37c"); // object validation above 37c
   }
   else if ((t <37) && (t >=30)){
   Serial.println("Object condition normal "); // object validaton between 30 - 37
   }
  else if (t <30){
    Serial.println("Object condition under 30c"); // object validation under 30
  }
  

   
  

  delay(1000);
  }
  void dsplay(){
   Serial.println("Data");
    pulse();
    suhu();
    
   Serial.println("");
   
    }
void loop() {
  Display();

}
