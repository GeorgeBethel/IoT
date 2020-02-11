//import neccesary libraries

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define Pump_switch 2
#define trig 4
#define echo 5

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "zTVdMzg45Af4ofhIkMIKCHKJyu2HIm4U";




// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "GEORGE";      //enter your wifi network name
char pass[] = "robot_freek1914";  // enter your wifi password

void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);

  pinMode(Pump_switch, OUTPUT);
  
  pinMode(trig,OUTPUT);
  
  pinMode(echo,INPUT);


  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80); //connects to blynk app
  
  Blynk.notify("there is power!");
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run(); //starts the blynk server
  water_level();
}



BLYNK_WRITE(V1) // Button Widget writes to Virtual Pin V1(pump virtual pin) 
{
  int pinValue1 = param.asInt();
  if(pinValue1 == 1) {     // if Button sends 1(when button is pressed)
    digitalWrite(Pump_switch, HIGH);             // switches on the pump
  }
  else
  
  { 
    digitalWrite(Pump_switch, LOW);             // switches off the pump
  }
}



void water_level(){
        
      int water_lev=calculat_water_level();

      //Serial.println(water_lev);  uncoment to get the value of the distance on the serial monitor  
      
      water_lev=map(water_lev,21,5,0,100); //maps the higher value(meaning no water in the tank)reading of the ultrasonic range sensor to 0 
                                            //and maps the lower value(meaning tank is full) to the higher value

      if (water_lev<100){
        
        Blynk.virtualWrite(V2, water_lev);  // pushes the vlevel of water to the level indicator on the app
        
        }
      else{                                              
        
        // the Blynk.virtualWrite(V2, water_lev) is not added here so that the level indicator value on blynk does not keep increasing
        
        Blynk.notify("water is full");  // pushes a notification when the tank is over 50% full
        }
      
      
      
      }



int calculat_water_level(){              //function to calculate the level of the water
  
  digitalWrite(trig,LOW);  // the trigger pin is set to low in other to get rid of the noise
  
  delay(10);
  
  digitalWrite(trig,HIGH);
  
  delay(10);
  
  digitalWrite(trig,LOW);
 
 int distance=pulseIn(echo,HIGH,150000)/50;    //calculates and distance
 
  return constrain(distance,1,300);            // constrains the distance value between 1 and 300
  
  
  }
