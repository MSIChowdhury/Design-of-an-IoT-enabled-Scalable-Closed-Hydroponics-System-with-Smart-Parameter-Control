#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Water Temperature Setup
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemperature(&oneWire);
float water_temperature = 0;


//EC
#define address 100              //default I2C ID number for EZO EC Circuit.

char computerdata[32];           //we make a 32 byte character array to hold incoming data from a pc/mac/other.
byte code = 0;                   //used to hold the I2C response code.
char ec_data[32];                //we make a 32 byte character array to hold incoming data from the EC circuit.
byte in_char = 0;                //used as a 1 byte buffer to store inbound bytes from the EC Circuit.
byte i = 0;                      //counter used for ec_data array.

float EC;
float Test_EC=0;  //float var used to hold the float value of the conductivity.

//EC

//pH
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 4                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be
 
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work
 
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float pH;  
//pH

//Ultrasonic Sensor
const int trigPin = 9;
const int echoPin = 10;

long duration;
long timer1;
long timer2;
long timer3;
float distance;
float waterLevel;
float groundLevel= 16.70;
//

//int count_flag = 0;


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
//  PH Setup
  myserial.begin(9600);                               //set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);  
  
  //Water Temperature Setup
  dallasTemperature.begin();

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  delay(1000);

}

//void serialEvent() {                                  //if the hardware serial port_0 receives a char
//  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
//  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
//}

void loop() {


//PH

timer1 = millis();
while(sensor_string_complete == false){


  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)myserial.read();              //get the char we just received

//    Serial.print(inchar);
//    Serial.println();
    sensorstring += inchar;                           //add the char to the var called sensorstring
//    Serial.print(sensorstring);
//    Serial.println();
    if (inchar == '\r' ) {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }
    if(millis() - timer1 >= 10000){
    Serial.print("This is pH loop: ");
    Serial.print(sensorstring);
    Serial.println();
  }
}

  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
//    Serial.println(sensorstring);                     //send that string to the PC's serial monitor
          //Serial.print("This is if-2 in pH");
      //Serial.println();
      //delay(5000);
                                              //uncomment this section to see how to convert the pH reading from a string to a float 
    if (isdigit(sensorstring[0])) {
            //Serial.print("This is if-3 in pH");
      //Serial.println();
      //delay(5000);//if the first character in the string is a digit
      pH = sensorstring.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
    }

    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

// PH



////EC
    Wire.beginTransmission(address);                                            //call the circuit by its ID number.
    Wire.write("r");                                                   //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                     //end the I2C data transmission.

    delay(570);                                                             //wait the correct amount of time for the circuit to complete its instruction.
    Wire.requestFrom(address, 32, 1);                                         //call the circuit and request 32 bytes (this could be too small, but it is the max i2c buffer size for an Arduino)
    code = Wire.read();                                                       //the first byte is the response code, we read this separately.
    timer2 = millis();
    while (Wire.available()) {
            //Serial.print("This is EC loop");
      //Serial.println();//are there bytes to receive.
      in_char = Wire.read();                   //receive a byte.
      //Serial.print(in_char);
      //Serial.println();
      ec_data[i] = in_char;                    //load this byte into our array.
      //Serial.print(ec_data);
      //Serial.println();
      i += 1;                                  //incur the counter for the array element.
      if (in_char == 0 || i == 31) {                      //if we see that we have been sent a null command.
              //Serial.print("This is if in EC");
      //Serial.println();
      
        i = 0;                                 //reset the counter i to 0.
        break;                                 //exit the while loop.
      }
          if(millis() - timer2 >= 10000){
    Serial.print("This is EC loop: ");
    Serial.print(ec_data);
    Serial.print(" ");
    Serial.print(i);
    Serial.println();
    
  }
    }
    
    EC=atof(ec_data); //Unit is micro-siemens/cm
  
//
//////EC



  // Waterproof Temperature Sensor START
        //Serial.print("This is water temp");
      //Serial.println();
  dallasTemperature.requestTemperatures();
  water_temperature = dallasTemperature.getTempCByIndex(0);

  //Waterproof Temperature Sensor END


  Serial.print(EC);
  Serial.print(",");
  
  Serial.print(pH);
  Serial.print(",");

  Serial.print(water_temperature);
  Serial.println();

}