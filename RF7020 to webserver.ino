/* Moo.id Gateway device( Arduino Uno - SIM800L - RF7020)
   Written By Muhammad Rafi for ERG Research Group 

   Licensed under the Computer University of Indonesia (UNIKOM) License, a copy of which
   should have been included with this software. 
   
   This sketch, for Receive data from Moo Tracker 
   and data will send to Web Server 

   Created Sep 2020
*/

#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(7, 8); // RX, TX

// a string to hold incoming data
String inputString = "";    

// whether the string is complete     
boolean stringComplete = false; 

// an array to contain outcome parse
String data, dt[10];     

// initialize variable 
String id,suhu,pulse,lon,lat,berat; 
boolean parsing = false;      
boolean  GSM_Ready = true;
int i, n;


void setup() {
  // format data = header, id, lat, lon, suhu, berat, pulse;
  data = id + "," + lat + "," + lon + "," + suhu + "," + berat + "," + pulse;
  
  // initialize serial 
  Serial.begin(9600);
  
  // reserve 500 bytes for the inputString
  inputString.reserve(500);
  
  // initialize serial for Sim800L
  gprsSerial.begin(19200);
  
  //print the string when gprs ready 
  Serial.println("Config SIM800L...");
  delay(2000);
  Serial.println("Done!...");
  stringComplete = false;
  parsing =false;
  GSM_Ready = true;
}

//========================================================================
void loop() {
  /*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
   if(Serial.available()>0) {
    while(Serial.available()){
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      data = inputString;
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      if (inChar == '\n') {
        parsing = true;
        delay(1000);
        }
     }
    //Print received data 
    Serial.print("Radio data : ");
    Serial.println(data);
    // validate parsing process
    if (parsing == true){
    parsingData();
      Serial.println("==========Mulai kirim ke web==============");
      Serial.println(id + lat + lon + suhu+ berat+pulse);
      gsm();
      inputString = "";
      data = "";
      parsing = true;
      }
    }

  }
  void gsm (){
    // attach or detach from GPRS service
    gprsSerial.println("AT");
    delay(1000);
    toSerial();
  
    // bearer settings
    gprsSerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    delay(1000);
    toSerial();
  
    // bearer settings
    gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"www\"");
    delay(2000);
    toSerial();
  
    // bearer settings
    gprsSerial.println("AT+SAPBR =1,1");
    delay(2000);
    toSerial();
  
    // bearer settings
    gprsSerial.println("AT+SAPBR=2,1");
    delay(2000);
    toSerial();
  
    // initialize http service
    gprsSerial.println("AT+HTTPINIT");
    delay(2000);
    toSerial();
  
    // set http param value
    //gprsSerial .println("AT+HTTPPARA=\"URL\",\"http://medion.ergnologi.com/Data_Alat/add.php?idsapi=3&latitude=-6.886576&longitude=107.615110&suhu="+String(suhu)+"&berat_badan=100&detak_jantung=273\"");
    //gprsSerial .println("AT+HTTPPARA=\"URL\",\"http://medion.ergnologi.com/Data_Alat/add.php?idsapi=3&latitude=-6.886576&longitude=107.615110&suhu=27&berat_badan=543&detak_jantung=256\"");
    //gprsSerial .println("AT+HTTPPARA=\"URL\",\"http://medion.ergnologi.com/Data_Alat/add.php?idsapi=3&latitude="+String (dt[1])+"&longitude="+String(dt[2])+"&suhu="+String(dt[3])+"&berat_badan="+String(dt[4])+"&detak_jantung="+String(dt[5])+"\"");
   // gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://medion.ergnologi.com/Data_Alat/add.php?idsapi=3&latitude="+lat+"&longitude="+lon+"&suhu="+suhu+"&berat_badan="+berat+"&detak_jantung="+pulse+"\"");
     gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://mooid.iamwisnu.xyz/Data_alat/add.php?idsapi=3&latitude="+lat+"&longitude="+lon+"&suhu="+suhu+"&berat_badan="+berat+"&detak_jantung="+pulse+"\"");
 
    //gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://192.168.0.14/gsm/receiver.php?lon="+String(14.123456)+"&lat="+String(127.1234567)+"/");
    delay(1000);
    toSerial();
  
    // set http action type 0 = GET, 1 = POST, 2 = HEAD
    gprsSerial.println("AT+HTTPPARA=\"CID\",1");
    delay(2000);
    toSerial();
  
    // read server response
    gprsSerial.println("AT+HTTPACTION=1");
    delay(1000);
    toSerial();
    GSM_Ready = true; 
}


void toSerial()
  //read gprs 
  { 
  while(gprsSerial.available()!=0)
    {
    Serial.write(gprsSerial.read());
    }
  }
void parsingData() {
  // hold incoming data 
  Serial.print("data masuk : ");
  Serial.print(data);
  Serial.print("\n");

  //parse data with comma and store it into array 
  int j = 0;
  dt[j] = "";
  for (i = 0; i < data.length(); i++) {
    if ((data[i] == '~') || (data[i] == ','))
    {
      j++;
      dt[j] = "";
    }
    else
    {
      dt[j] = dt[j] + data[i];
    }
  }
  //print outcome of parsing 
  Serial.println("Hasil Parsing: ");
  Serial.print("data ID: ");
  
  //print id data
  id = String (dt[1]);
  Serial.println(id);
  
  //print latitude data 
  lat = String (dt[2]);
  Serial.print("data Lat: ");
  Serial.println(lat);

  //print longitude data
  lon = String (dt[3]);
  Serial.print("data lon: ");
  Serial.println(lon);

  //print temperature data
  Serial.print("data suhu: ");
  suhu = String (dt[4]);
  Serial.println(suhu);

  //print weight data 
  berat = String (dt[5]);
  Serial.print("data berat: ");
  Serial.println(berat);

  //print pulse data 
  pulse = String (dt[6]);
  Serial.print("data pulse: ");
  Serial.println(pulse);
}
