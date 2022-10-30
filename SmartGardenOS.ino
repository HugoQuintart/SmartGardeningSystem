
/*
         __     __   __      __   __________       _______________
        /  /   /  / /  /    /  / /  _______/      /   ________    /
       /  /   /  / /  /    /  / /  /             /  /         /  /
      /  /___/  / /  /    /  / /  /     ______  /  /         /  /
     /  ____   / /  /    /  / /  /     /   __/ /  /         /  /
    /  /   /  / /  /    /  / /  /      /  /   /  /         /  /
   /  /   /  / /  /____/  / /  /______/  /   /  /_________/  /
  /__/   /__/ /__________/ /____________/   /_______________/
  This is the operating OS of my project
*/
// preparing the lcd screen
#include <LiquidCrystal.h> //including the library to use the screen
LiquidCrystal lcd(11,9,6,5,4,3);

// preparing the temperature sensor
#include <DallasTemperature.h> //including the libraries to use the sensor
#include <OneWire.h> //including the libraries to use the sensor

#define ONE_WIRE_BUS 39 //the communication pin (bus) is set on the pin 39
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// preparing the DHT sensor
#include "DHT.h"   // include the library
#define DHTPIN 8    // This is where the sensor should be plugged
#define DHTTYPE DHT22 // I choosed the DHT22 sensor bc it is more precise, but you can use a DHT11 or wathever.
DHT dht(DHTPIN, DHTTYPE);

//Calibrating the soil moisture sensors
const int wet = 0;// Setting the "wet" value to 0 according to the experiment (0 => 0V on the analog input and 1023 => 5V on the analog input)
const int dry = 1023; // Setting the "dry" hygrometry value according to the experiment (0 => 0V on the analog input and 1023 => 5V on the analog input)

//Setting up the moisture analysis:
int moisturePin1 = A0; // The first soil moisture sensor is plugged on the A0 pin of the Arduino board
int moisturePin2 = A1; // The second soil moisture sensor is plugged on the A1 pin of the Arduino board
int moisturePin3 = A2; // The third soil moisture sensor is plugged on the A2 pin of the Arduino board
int moisturePin4 = A3; // The fourth soil moisture sensor is plugged on the A3 pin of the Arduino board
int moisturePin5 = A4; // The fifth soil moisture sensor is plugged on the A4 pin of the Arduino board
int moistureValue; // initializing the value which correspond to the soil moisture
int moistureUsable; // initializing the value to store the value of the average of the sensors values
int hygroVal; // Initialize the variable that stores the soil moisture percentage.


//Setting up the watering process
int dryingPin = 41; // The water suction pump is plugged on the pin 41 (by a relay bc the pump runs on 12VDC and the arduino can only provide 5VDC).
int wateringPin = 42; // The water suction pump is plugged on the pin 42 (by a relay bc the pump runs on 12VDC and the arduino can only provide 5VDC).
int wateringState; //Initialize the variable that stores the state of the watering pin (1 or 0).
int dryingState; // initialiser la variable qui va stocker l'état de la pompe de reprise.

//préparation des variables pour l'analyse de la température du sol

//et le reste des variables:
int statePin = 13; // la led d'état de la carte électronique sera la led sur la pin 13

void setup() {
  moistureValue = 0; // Définir la valeur moistureValue à 0

  pinMode(moisturePin1, INPUT); // Définir la pin du capteur d'hygrométrie comme entrée
  pinMode(moisturePin2, INPUT); // Définir la pin du capteur d'hygrométrie comme entrée
  pinMode(moisturePin3, INPUT); // Définir la pin du capteur d'hygrométrie comme entrée
  pinMode(moisturePin4, INPUT); // Définir la pin du capteur d'hygrométrie comme entrée
  pinMode(wateringPin, OUTPUT); // Définir la pin de la pompe d'arrosage comme sortie
  pinMode(dryingPin, OUTPUT); // Définir la pin de la pompe de reprise en tant que sortie
  pinMode(statePin, OUTPUT); // La led d'état correspond à la pin 13, en sortie de courant

  digitalWrite(statePin, HIGH);

  Serial.begin(9600); // Initialiser le moniteur série en 9600 BAUD.
  lcd.begin(20, 4); // Initialiser l'écran en lcd 20x4 (20 caractères sur 4 lignes)
  dht.begin(); //démarrer la dht (pour mesures de l'humidité et de la température)
  sensors.begin();
  // Séquence de démarrage (nom SE, infos, Créateur)
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

  // ***********************************************partie DHT***********************************************
lcd.clear(); // effacer l'écran lcd
  
  // Lecture du taux d'humidité
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();
  // Pour lire la température en Fahrenheit
  float f = dht.readTemperature(true);

  // Stop le programme et renvoie un message d'erreur si le capteur ne renvoie aucune mesure
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from the DHT sensor !");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DHT sensor error");
    lcd.setCursor(0,1);
    lcd.print("Please check");
    lcd.setCursor(0,2);
    lcd.print("the connection");
    delay(1000);
    lcd.clear();
    return;
  }

  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  float hi = dht.computeHeatIndex(f, h);
  // afficher dans le moniteur série
  Serial.print("Hygrometry: ");
  Serial.print(h);
  Serial.print(" %, ");
  Serial.print("Outdoor temperature: ");
  Serial.print(t);
  Serial.print(" *C,  ");
  // afficher sur l'écran
  lcd.setCursor(0, 0);
  lcd.print("Hygrometry:");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0, 2);
  lcd.print("Outdoor temp: ");
  lcd.setCursor(0,3);
  lcd.print(t);
  lcd.print(" *C");
  delay(2000); // définir un arrêt de 2 secondes pour faciliter la lecture sur l'écran
  

 
  // ***********************************************partie arrosage**********************************************
  moistureValue = (analogRead(moisturePin1) + analogRead(moisturePin2) + analogRead(moisturePin3) + analogRead(moisturePin4) + analogRead(moisturePin5));
  moistureUsable = moistureValue / 5;
  hygroVal = map(moistureUsable, dry, wet, 0, 100); // on défini les différents seuils initialisés au départ comme les seuils 0 et 100%
  
       // enclencher la pompe d'arrosage si l'humidité du sol est inférieure à 65%
      if(hygroVal <= 65){
 Serial.println("wateringPin, HIGH");
        digitalWrite(wateringPin, HIGH);
        wateringState = 1;
      }
      // enclencher la pompe de reprise si l'humidité du sol dépasse les 90%
      if(hygroVal >= 90){
 Serial.println("dryingPin, HIGH");
        digitalWrite(dryingPin, HIGH);
        dryingState = 1;
      }
      // arrêter les pompes quand on se situe à nouveau dans la fourchette adaptée à la plante.
      if (hygroVal  >= 80) {
        if (hygroVal <= 74) {
 Serial.println("wateringPin, LOW");
          digitalWrite(wateringPin, LOW);
          wateringState = 0;
 Serial.println("dryingPin, LOW");
          digitalWrite(dryingPin, LOW);
          dryingState = 0;
        }
      }
      sensors.requestTemperatures(); // Envoyer l'ordre au capteur de donner la température un peu avant de l'afficher pour éviter les latences potentielles
  // On décode les données du capteur de température pour les avoir sous forme de données que nous pourrions interpréter
  float tempC = sensors.getTempCByIndex(0);

   // ***********************************************partie hygrométrie***********************************************

  moistureValue = (analogRead(moisturePin1) + analogRead(moisturePin2) + analogRead(moisturePin3) + analogRead(moisturePin4) + analogRead(moisturePin5));
  moistureUsable = moistureValue / 5;
  hygroVal = map(moistureUsable, dry, wet, 0, 100); // on défini les différents seuils initialisés au départ comme les seuils 0 et 100%
  Serial.print("Humidite du sol: ");
  Serial.print(hygroVal);
  Serial.print(" %, ");
  
  lcd.clear();
  // On s'occupe de l'humidité
  lcd.setCursor(0, 0);
  lcd.print("Humidite du sol: ");
  lcd.setCursor(0,1);
  lcd.print(hygroVal);
  lcd.print("%");
  
  // ***********************************************partie mesure de la température***********************************************

  // Check if reading was successful
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperature du sol:  ");
    Serial.print(tempC);
    Serial.println(" C, ");
    lcd.setCursor(0, 2);
    lcd.print("Temperature du sol: ");
    lcd.setCursor(0,3);
    lcd.print(tempC);
    lcd.print(" *C");
    delay(2000); // définir un arrêt de 2 seconde pour faciliter la lecture sur l'écran
  }
  else
  {
    lcd.setCursor(0,2);
    lcd.print("Pas de sonde de t*.");
    Serial.println("Error: Could not read temperature data");
  }
  
}
