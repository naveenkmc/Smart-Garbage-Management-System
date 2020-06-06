#include <WiFi.h>                                                    // esp8266 library

#include <IOXhop_FirebaseESP32.h>                                                // firebase library
#include <DHT.h>                                                            // dht11 temperature and humidity sensor library

#define FIREBASE_HOST "https://cleanify-01.firebaseio.com/"                          // the project name address from firebase id
#define FIREBASE_AUTH "4Iql6gCDxEG3rxoDcZKrjT2RkHY0Es19GlisCZc4"            // the secret key generated from firebase

#define WIFI_SSID "Connect"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "zxcvbnma"                                    //password of wifi ssid
 
#define DHTPIN 22                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE); 

const int trigPin = 12;
const int echoPin = 14;

long duration;
int distance;
 int GarbageLevel;
void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  dht.begin();                                                               //Start reading dht sensor
}

void loop() { 
  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
    
  if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("C");
  String fireTemp = String(t) + String("C");                                                     //convert integer temperature to string temperature
  

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
   GarbageLevel = ((22-distance)*100)/22;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(GarbageLevel);
  delay(1000);
  Firebase.set("/1111/Humidity", fireHumid);                                  //setup path and send readings
  Firebase.set("/1111/Temperature", fireTemp);                              //setup path and send readings
  Firebase.set("/1111/GarbageLevel", GarbageLevel);
}
