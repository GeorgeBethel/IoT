//import neccesary libraries

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int Pump_switch=2;
int trig=4;
int echo=5;
int water_lev;
boolean IsFull;
int water_lev_interrupt=27;
String Mode="";

BlynkTimer timer;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "zTVdMzg45Af4ofhIkMIKCHKJyu2HIm4U";




// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "GEORGE";      //enter your wifi network name
char pass[] = "robot_freek1914";  // enter your wifi password


void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

void Mode_set(){  // function to turn of pump if automatic mode is selected
  
  if(Mode=="auto"){

    if(IsFull==true){
    
    digitalWrite(Pump_switch,LOW);
    Serial.println("pump off");
    }
  else if(IsFull==false){
    
    digitalWrite(Pump_switch,HIGH);
    Serial.println("pump on");
    }
  }
  }

void setup()
{
  // Debug console
  Serial.begin(9600);
  timer.setInterval(1000L, myTimerEvent);

  //Blynk.begin(auth, ssid, pass);

  pinMode(Pump_switch, OUTPUT);
  pinMode(water_lev_interrupt,OUTPUT);
  pinMode(trig,OUTPUT);
  
  pinMode(echo,INPUT);

  
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80); //connects to blynk app
  
  Blynk.notify("there is power!");
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  if(calculat_water_level()>=100){

    IsFull=true;
    digitalWrite(Pump_switch,LOW);
    }
   else{
    
    IsFull=false;
    digitalWrite(Pump_switch,HIGH);
    }
}

void loop()
{
  Blynk.run(); //starts the blynk server
  timer.run();
  water_level();
  Mode_set();
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
  


  
BLYNK_WRITE(V6) // Button Widget writes to Virtual Pin V6(Auto/manual pump control) 
{
  int pinValue2 = param.asInt();

  if(pinValue2 == 1) {     // if Button sends 1(when button is pressed)
    Serial.println("auto");
    Mode="auto";
    //digitalWrite(Pump_switch, HIGH);             // switches on the pump
  }
  else
  
  { 
    Serial.println("manual");
    Mode="manual";
    //digitalWrite(Pump_switch, LOW);             // switches off the pump
  }
}



void water_level(){
        
      water_lev=calculat_water_level();

      Serial.println(water_lev);  //uncoment to get the value of the distance on the serial monitor  
      
      water_lev=map(water_lev,21,5,0,100); //maps the higher value(meaning no water in the tank)reading of the ultrasonic range sensor to 0 
                                            //and maps the lower value(meaning tank is full) to the higher value
      Serial.println("this is the mapped level: ");
      Serial.println(water_lev);
      if (water_lev<100){
        
        Blynk.virtualWrite(V2, water_lev);  // pushes the vlevel of water to the level indicator on the app
        IsFull=false;
        
        }
      else if(water_lev>=100){                                              
        
        // the Blynk.virtualWrite(V2, water_lev) is not added here so that the level indicator value on blynk does not keep increasing
        IsFull=true;
        
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
