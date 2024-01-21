#include <Arduino.h>
#include "MHZ19.h" 

//Air Temperature and Humidity Sensor
#define DHT11_PIN 7
#define DHT22_PIN 5
DHT dht11(DHT11_PIN, DHT11);
DHT dht22(DHT22_PIN, DHT22);

MHZ19 myMHZ19;                                             // Constructor for library

unsigned long getDataTimer = 0;
int CO2;

//Write on Mega from Python
const int bufferSize = 4; // Adjust this according to your needs
char inputString[bufferSize]; // A character array to store the incoming string
char *token; // A pointer to help with parsing
int pinNumber, pinState;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  pinMode(50,OUTPUT);
  pinMode(48,OUTPUT);
  pinMode(46,OUTPUT);
  pinMode(44,OUTPUT);
  pinMode(42,OUTPUT);
  pinMode(40,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(36,OUTPUT);
  pinMode(34,OUTPUT);
  
  digitalWrite(50, LOW);
  digitalWrite(48, HIGH);
  digitalWrite(46, HIGH); 
  digitalWrite(44, HIGH); 
  digitalWrite(42, HIGH); 
  digitalWrite(40, HIGH); 
  digitalWrite(38, HIGH); 
  digitalWrite(36, HIGH); 
  digitalWrite(34, HIGH);  

  //Air Temperature and Humidity Sensor 
  dht11.begin();
  dht22.begin();
   
  myMHZ19.begin(Serial1);                                // *Serial(Stream) refence must be passed to library begin(). 
  myMHZ19.autoCalibration(false);                              // Turn auto calibration ON (OFF autoCalibration(false))
  delay(1000);
    
  for(int j=0; j<=5; j++){
    CO2 = myMHZ19.getCO2();
    delay(100);
  } 
}

void loop() {
  
    if (millis() - getDataTimer >= 1100)
    {
        CO2 = myMHZ19.getCO2();                      
      
        //This Section measures air temperature and humidity start
        float humidity_1 = dht22.readHumidity();
        float humidity_2 = dht11.readHumidity();
        // Read temperature as Celsius (the default)
        float air_temperature_1 = dht22.readTemperature();
        float air_temperature_2 = dht11.readTemperature();

        getDataTimer = millis();

//      Serial.print("Humidity: ");
//      Serial.println(humidity);
//      Serial.print("Air Temperature: ");
//      Serial.print(air_temperature);
//      Serial.println(" C");

      if(CO2 < 2000){
        digitalWrite(38, LOW);
      }
      else if(CO2>=2000){
        digitalWrite(38, HIGH);
      }
        digitalWrite(50, LOW);
        
        Serial.print(CO2);
        Serial.print(",");

        Serial.print(air_temperature_1);
        Serial.print(",");
        Serial.print(air_temperature_2);
        Serial.print(",");
          
        Serial.print(humidity_1);
        Serial.println(',');
        Serial.print(humidity_2);
        Serial.println();

    }
    
  
  if(Serial.available()!=0){
    Serial.readBytesUntil('\n', inputString, bufferSize);
  
  // Parse the inputString using strtok to split it into tokens
  token = strtok(inputString, ",");
    
  // Check if the token is not null (indicating that it found a comma)
  if (token != NULL) {
    // Convert the first token (pin number) to an integer
    pinNumber = atoi(token);
      
    // Move to the next token
    token = strtok(NULL, ",");
      
    // Check if the second token is not null (indicating that it found another comma)
    if (token != NULL) {
      // Convert the second token (pin state) to an integer
      pinState = atoi(token);
        
      // Now, you have pinNumber and pinState as separate variables
      // You can use them in your Arduino code as needed
      // For example, you can digitalWrite(pinNumber, pinState) to set the pin stat
      digitalWrite(pinNumber, pinState);
      }
    }
  }
  
}