#include <WiFi.h>                                                    

#include <IOXhop_FirebaseESP32.h>                                                
#include <DHT.h>                                                            


#define FIREBASE_HOST = " "                          
#define FIREBASE_AUTH = " "            

#define WIFI_SSID = " "                                             
#define WIFI_PASSWORD = " "                                  

//Temp 
#define DHTPIN 22                                                         
#define DHTTYPE DHT11                                                      
DHT dht(DHTPIN, DHTTYPE); 


//Ultra Sonic
const int trigPin = 12;
const int echoPin = 14;

int buzzer = 10;
long duration;
int distance;
int GarbageLevel;


//Flame sensor
int const flame = 5;


//Gas sensor  
int redLed = 12;
int greenLed = 11;
int smokeA0 = A5;
int sensorThres = 400;


//PIR
int blueled = 13;
int sensor = 6;
int state = LOW;
int val = 0;


void setup() 
{
  pinMode(buzzer, OUTPUT);
    
  //Flame
  pinMode(flame, INPUT);


  //Gas
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(smokeA0, INPUT);


  //PIR
  pinMode(blueled, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);


  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                    
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            
  dht.begin();                                                               
}

void loop() { 



 //Flame
{  
  if (flame == HIGH)
  {
    Serial.print("flame is absent \n");
    {
      digitalWrite(buzzer, HIGH);
      delay(1000);
    }
  }

  else 
  {
    Serial.print("flame is present \n");
     digitalWrite(buzzer, LOW);
   
  }
}
{
  //Gas
  { 
    int analogSensor = analogRead(smokeA0);

    Serial.print("Gas Density: ");
    Serial.println(analogSensor);
    if (analogSensor > sensorThres)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(buzzer, HIGH);
      digitalWrite(greenLed, LOW);
      
    }
    else
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      digitalWrite(buzzer, LOW);
      
    }
    
  }
}  
  //PIR
{  
  {
    val = digitalRead(sensor);
    if (val == HIGH) 
    {
      {
        digitalWrite(blueled, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(1000);
      }     
      

      if (state == LOW) 
      {
        Serial.println("Motion detected! \n");
        state = HIGH;
      }
    }
    else 
    {
      digitalWrite(blueled, LOW);
      if (state == HIGH) 
      {
        Serial.println("Motion stopped! \n");
        digitalWrite(buzzer, LOW);
        state = LOW;       
      }
    }
  }
}
  //Temp
  float h = dht.readHumidity();                                              
  float t = dht.readTemperature();                                           
    
  if (isnan(h) || isnan(t)) 
  {                                                
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                       
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("C");
  String fireTemp = String(t) + String("C");                                                    
  

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance= duration*0.034/2;
   GarbageLevel = ((22-distance)*100)/22;
  
  Serial.print("Distance: ");
  Serial.println(GarbageLevel);
  delay(1000);
  

  Firebase.set("/Alerts/1111/Humidity", fireHumid);                                 
  Firebase.set("/Alerts/1111/Temperature", fireTemp);                              
  Firebase.set("/Alerts/1111/GarbageLevel", GarbageLevel);
  
}
