
/*
         __     __   __      __   __________       _______________
        /  /   /  / /  /    /  / /  _______/      /   ________    /
       /  /   /  / /  /    /  / /  /             /  /         /  /
      /  /___/  / /  /    /  / /  /     ______  /  /         /  /
     /  ____   / /  /    /  / /  /     /   __/ /  /         /  /
    /  /   /  / /  /    /  / /  /      /  /   /  /         /  /
   /  /   /  / /  /____/  / /  /______/  /   /  /_________/  /
  /__/   /__/ /__________/ /____________/   /_______________/

  THis is the operating system of my project
*/
// prepare the LCD screen
#include <LiquidCrystal.h> //including a library to use the screen
LiquidCrystal lcd(11,9,6,5,4,3); //define the lcd communication buses

// including the two libraries to use the DS18B20 temperature sensor
#include <DallasTemperature.h>
#include <OneWire.h>
// prepare the temp sensor
#define ONE_WIRE_BUS 39 // define the communication pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// prepare the DHT sensor
#include "DHT.h"   // including the library
#define DHTPIN 8    // define the communication pin
#define DHTTYPE DHT22      // define the DHT type
DHT dht(DHTPIN, DHTTYPE);

//calibrate the moisture sensors
const int wet = 0;// Setting a "wet value" depending of the recieved signal on the analog pin (1023 => 5V and 0 => 0V)
const int dry = 1023; // Setting a "dry value" depending of the recieved signal on the analog pin (1023 => 5V and 0 => 0V)
//prepare the soil moisture sentors:
int moisturePin1 = A0; // Set the soil moisture sensor 1 on the pin A0
int moisturePin2 = A1; // Set the soil moisture sensor 2 on the pin A1
int moisturePin3 = A2; // Set the soil moisture sensor 3 on the pin A2
int moisturePin4 = A3; // Set the soil moisture sensor 4 on the pin A3
int moisturePin5 = A4; // Set the soil moisture sensor 5 on the pin A4
int moistureValue; // initialize the soil moisture value (brut)
int moistureUsable; // initalize the variable that stores the average of the sensors values.
int hygroVal; // initialize the variable that stores the percentage of humidity.


//prepare some variables for the watering function:
int dryingPin = 41; // Setting the draining pump on the 41st pin
int wateringPin = 42; // Setting the watering pump on the 42nd pinµ
//those variables are for debugging purposes
int wateringState; 
int dryingState; 

int statePin = 13; // la led d'état de la carte électronique sera la led sur la pin 13

void setup() {
  moistureValue = 0; // define the moistureValue to 0

  pinMode(moisturePin1, INPUT); // Set the hygrometry sensor as an input.
  pinMode(moisturePin2, INPUT); // Set the hygrometry sensor as an input.
  pinMode(moisturePin3, INPUT); // Set the hygrometry sensor as an input.
  pinMode(moisturePin4, INPUT); // Set the hygrometry sensor as an input.
  pinMode(wateringPin, OUTPUT); // Define the relay of the watering pump as an output
  pinMode(dryingPin, OUTPUT); // Define the relay of the draining pump as an output
  pinMode(statePin, OUTPUT); 

  digitalWrite(statePin, HIGH);

  Serial.begin(9600); // Initialize the serial monitor in 9600 baud
  lcd.begin(20, 4); // Define the LCD screen as a 20x4 screen (4 lines and 20 columns)
  dht.begin(); //start the DHT sensor
  sensors.begin();
  // Booting payload (please consider not modifying those lines)
  lcd.begin(20,4);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("SMART GARDEN");
  lcd.setCursor(0,1);
  lcd.print("BOOTING, PLEASE WAIT");
  lcd.setCursor(0, 3);
  lcd.print("by: HQ Developpement");
  delay(3000);
  lcd.clear();


}

void loop() {

  // ***********************************************DHT***********************************************
lcd.clear(); // effacer l'écran lcd
  
  // Reading humidity value
  float h = dht.readHumidity();
  // Reading temperature in Celcius
  float t = dht.readTemperature();
  // Reading temp in Farenheit
  float f = dht.readTemperature(true);

  // This script will run if the DHT isn't connected
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from the DHT sensor, please check the connection");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DHT sensor error");
    lcd.setCursor(0,1);
    lcd.print("Please check the");
    lcd.setCursor(0,2);
    lcd.print("connection");
    delay(500);
    lcd.clear();
    return;
  }

 
  float hi = dht.computeHeatIndex(f, h);
  // Printing datas in the serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %, ");
  Serial.print("Outdoor temperature: ");
  Serial.print(t);
  Serial.print(" *C,  ");
  // Displaying on the screen
  lcd.setCursor(0, 0);
  lcd.print("Outdoor humidity:");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0, 2);
  lcd.print("Outdoor temp: ");
  lcd.setCursor(0,3);
  lcd.print(t);
  lcd.print(" *C");
  delay(2000); // 2second delay to make the screen reading easier
  

 
  // ***********************************************Watering**********************************************
  moistureValue = (analogRead(moisturePin1) + analogRead(moisturePin2) + analogRead(moisturePin3) + analogRead(moisturePin4) + analogRead(moisturePin5));
  moistureUsable = moistureValue / 5;
  hygroVal = map(moistureUsable, dry, wet, 0, 100); // define the "wet" and "dry" variables as 0 and 100% of humidity.
  
       // turn on the watering pump if the humidity is inferior to 70 %
      if(hygroVal <= 70){
        digitalWrite(wateringPin, HIGH);
        wateringState = 1;
      }
      // turn on the draining pump if the humidity is superior to 90%
      if(hygroVal >= 90){
        digitalWrite(dryingPin, HIGH);
        dryingState = 1;
      }
      // Turn off the pumps when the humidity is good.
      if (hygroVal  >= 80) {
        if (hygroVal <= 71) {
 Serial.println("wateringPin, LOW");
          digitalWrite(wateringPin, LOW);
          wateringState = 0;
 Serial.println("dryingPin, LOW");
          digitalWrite(dryingPin, LOW);
          dryingState = 0;
        }
      }
      sensors.requestTemperatures(); // Requesting the soil temp
  // Decoding the recieved datas
  float tempC = sensors.getTempCByIndex(0);

   // ***********************************************Soil moisture analysis***********************************************

  moistureValue = (analogRead(moisturePin1) + analogRead(moisturePin2) + analogRead(moisturePin3) + analogRead(moisturePin4) + analogRead(moisturePin5));
  moistureUsable = moistureValue / 5;
  hygroVal = map(moistureUsable, dry, wet, 0, 100); // Using the "wet" and "dry" variables to define the percentage of humidity.
  Serial.print("Soil moisture: ");
  Serial.print(hygroVal);
  Serial.print(" %, ");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil moisture: ");
  lcd.setCursor(0,1);
  lcd.print(hygroVal);
  lcd.print("%");
  
  // ***********************************************ground temperature***********************************************

  // Check if the temp reading was successful
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Soil temperature:  ");
    Serial.print(tempC);
    Serial.println(" C, ");
    lcd.setCursor(0, 2);
    lcd.print("Soil temperature: ");
    lcd.setCursor(0,3);
    lcd.print(tempC);
    lcd.print(" *C");
    delay(2000);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("temp sensor error");
    lcd.setCursor(0,1);
    lcd.print("Please check the");
    lcd.setCursor(0,2);
    lcd.print("connection");
    Serial.println("Error: Could not read temperature data.");
    delay(500);
    return;
  }
}
