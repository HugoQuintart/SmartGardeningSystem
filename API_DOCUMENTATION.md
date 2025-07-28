# Smart Garden OS - API Documentation

## Table of Contents
- [Overview](#overview)
- [Hardware Components](#hardware-components)
- [Pin Configuration](#pin-configuration)
- [Libraries and Dependencies](#libraries-and-dependencies)
- [Global Variables](#global-variables)
- [Functions](#functions)
- [Sensor APIs](#sensor-apis)
- [Control Systems](#control-systems)
- [Display System](#display-system)
- [Usage Examples](#usage-examples)
- [Troubleshooting](#troubleshooting)

## Overview

Smart Garden OS is an Arduino-based automated gardening system that monitors environmental conditions and automatically waters plants based on soil moisture levels. The system features multiple sensors for comprehensive monitoring and a dual-pump system for precise water management.

### Key Features
- **Multi-sensor monitoring**: Soil moisture, soil temperature, air temperature, and humidity
- **Automated watering**: Intelligent pump control based on soil moisture levels
- **Real-time display**: 20x4 LCD screen showing current readings
- **Serial monitoring**: Data logging for external analysis
- **Drainage system**: Prevents overwatering with automatic drainage

## Hardware Components

| Component | Model/Type | Pin(s) | Purpose |
|-----------|------------|--------|---------|
| Arduino | Mega 2560 | - | Main controller |
| LCD Display | 20x4 Character LCD | 11,9,6,5,4,3 | Data display |
| Soil Temperature | DS18B20 | 39 | Soil temperature monitoring |
| Air Sensor | DHT22 | 8 | Air temperature and humidity |
| Soil Moisture | Analog sensors (5x) | A0-A4 | Soil moisture monitoring |
| Water Pump | 12V Pump | 42 | Plant watering |
| Drain Pump | 12V Pump | 41 | Excess water drainage |
| Status LED | Built-in LED | 13 | System status indicator |
| Relays | 5V Relay Module | 41, 42 | Pump control |

## Pin Configuration

### Digital Pins
```cpp
// LCD Display Pins
#define LCD_RS    11
#define LCD_EN    9
#define LCD_D4    6
#define LCD_D5    5
#define LCD_D6    4
#define LCD_D7    3

// Sensor Pins
#define DHT_PIN       8    // DHT22 sensor
#define TEMP_PIN      39   // DS18B20 temperature sensor
#define STATUS_LED    13   // Built-in status LED

// Control Pins
#define WATERING_PIN  42   // Water pump relay
#define DRAINING_PIN  41   // Drain pump relay
```

### Analog Pins
```cpp
#define MOISTURE_PIN_1  A0  // Soil moisture sensor 1
#define MOISTURE_PIN_2  A1  // Soil moisture sensor 2
#define MOISTURE_PIN_3  A2  // Soil moisture sensor 3
#define MOISTURE_PIN_4  A3  // Soil moisture sensor 4
#define MOISTURE_PIN_5  A4  // Soil moisture sensor 5
```

## Libraries and Dependencies

### Required Libraries
```cpp
#include <LiquidCrystal.h>    // LCD display control
#include <DallasTemperature.h> // DS18B20 temperature sensor
#include <OneWire.h>          // OneWire communication protocol
#include <DHT.h>              // DHT22 sensor library
```

### Installation
```bash
# Arduino IDE Library Manager
# Install the following libraries:
# - LiquidCrystal (built-in)
# - DallasTemperature by Miles Burton
# - OneWire by Jim Studt
# - DHT sensor library by Adafruit
```

## Global Variables

### Sensor Calibration
```cpp
const int wet = 0;      // Wet soil analog value (0V)
const int dry = 1023;   // Dry soil analog value (5V)
```

### Sensor Variables
```cpp
int moistureValue;   // Raw moisture sensor reading (sum of all sensors)
int moistureUsable;  // Average moisture value (moistureValue / 5)
int hygroVal;        // Moisture percentage (0-100%)
```

### Control Variables
```cpp
int wateringState;   // Current watering pump state (0=OFF, 1=ON)
int dryingState;     // Current draining pump state (0=OFF, 1=ON)
```

### Hardware Objects
```cpp
LiquidCrystal lcd(11,9,6,5,4,3);           // LCD display object
OneWire oneWire(ONE_WIRE_BUS);              // OneWire bus object
DallasTemperature sensors(&oneWire);        // Temperature sensor object
DHT dht(DHTPIN, DHTTYPE);                   // DHT sensor object
```

## Functions

### setup()
**Purpose**: Initialize all hardware components and display boot screen

**Parameters**: None

**Returns**: void

**Description**: 
- Configures pin modes for all sensors and actuators
- Initializes serial communication at 9600 baud
- Sets up LCD display (20x4)
- Starts DHT and DS18B20 sensors
- Displays boot screen with project information

**Example**:
```cpp
void setup() {
  // Pin configuration
  pinMode(moisturePin1, INPUT);
  pinMode(wateringPin, OUTPUT);
  pinMode(dryingPin, OUTPUT);
  pinMode(statePin, OUTPUT);
  
  // Initialize components
  Serial.begin(9600);
  lcd.begin(20, 4);
  dht.begin();
  sensors.begin();
  
  // Boot screen display
  displayBootScreen();
}
```

### loop()
**Purpose**: Main program loop executing sensor readings and control logic

**Parameters**: None

**Returns**: void

**Description**:
- Reads DHT22 sensor data (air temperature and humidity)
- Processes soil moisture from 5 sensors
- Controls watering and drainage pumps
- Reads soil temperature
- Updates LCD display
- Logs data to serial monitor

**Execution Flow**:
1. Clear LCD display
2. Read and validate DHT sensor data
3. Display outdoor conditions
4. Process soil moisture readings
5. Execute pump control logic
6. Read soil temperature
7. Update display with soil conditions
8. Repeat cycle

## Sensor APIs

### DHT22 Air Sensor API

#### readHumidity()
```cpp
float h = dht.readHumidity();
```
- **Returns**: Air humidity percentage (0-100%)
- **Error**: Returns NaN if sensor disconnected

#### readTemperature()
```cpp
float t = dht.readTemperature();      // Celsius
float f = dht.readTemperature(true);  // Fahrenheit
```
- **Returns**: Air temperature in specified unit
- **Error**: Returns NaN if sensor disconnected

#### computeHeatIndex()
```cpp
float hi = dht.computeHeatIndex(f, h);
```
- **Parameters**: Temperature (°F), Humidity (%)
- **Returns**: Heat index value

### DS18B20 Soil Temperature API

#### requestTemperatures()
```cpp
sensors.requestTemperatures();
```
- **Purpose**: Request temperature reading from sensor
- **Returns**: void

#### getTempCByIndex()
```cpp
float tempC = sensors.getTempCByIndex(0);
```
- **Parameters**: Sensor index (0 for first sensor)
- **Returns**: Temperature in Celsius
- **Error**: Returns DEVICE_DISCONNECTED_C if sensor disconnected

### Soil Moisture Sensor API

#### readMoisture()
```cpp
// Read all 5 moisture sensors
int totalMoisture = analogRead(moisturePin1) + 
                   analogRead(moisturePin2) + 
                   analogRead(moisturePin3) + 
                   analogRead(moisturePin4) + 
                   analogRead(moisturePin5);

int averageMoisture = totalMoisture / 5;
int moisturePercent = map(averageMoisture, dry, wet, 0, 100);
```
- **Returns**: 
  - `totalMoisture`: Sum of all sensor readings (0-5115)
  - `averageMoisture`: Average sensor reading (0-1023)
  - `moisturePercent`: Moisture percentage (0-100%)

## Control Systems

### Automatic Watering System

#### Watering Logic
```cpp
// Watering thresholds
#define WATERING_THRESHOLD  70  // Start watering below 70%
#define DRAINAGE_THRESHOLD  90  // Start draining above 90%
#define OPTIMAL_MIN        71   // Optimal range minimum
#define OPTIMAL_MAX        80   // Optimal range maximum
```

#### Control Algorithm
```cpp
if (hygroVal <= WATERING_THRESHOLD) {
    digitalWrite(wateringPin, HIGH);  // Start watering
    wateringState = 1;
}

if (hygroVal >= DRAINAGE_THRESHOLD) {
    digitalWrite(dryingPin, HIGH);    // Start draining
    dryingState = 1;
}

if (hygroVal >= OPTIMAL_MIN && hygroVal <= OPTIMAL_MAX) {
    digitalWrite(wateringPin, LOW);   // Stop watering
    digitalWrite(dryingPin, LOW);     // Stop draining
    wateringState = 0;
    dryingState = 0;
}
```

### Pump Control API

#### startWatering()
```cpp
void startWatering() {
    digitalWrite(wateringPin, HIGH);
    wateringState = 1;
}
```

#### stopWatering()
```cpp
void stopWatering() {
    digitalWrite(wateringPin, LOW);
    wateringState = 0;
}
```

#### startDraining()
```cpp
void startDraining() {
    digitalWrite(dryingPin, HIGH);
    dryingState = 1;
}
```

#### stopDraining()
```cpp
void stopDraining() {
    digitalWrite(dryingPin, LOW);
    dryingState = 0;
}
```

## Display System

### LCD Display API

#### displayBootScreen()
```cpp
void displayBootScreen() {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("SMART GARDEN");
    lcd.setCursor(0, 1);
    lcd.print("BOOTING, PLEASE WAIT");
    lcd.setCursor(0, 3);
    lcd.print("by: HQ Developpement");
    delay(3000);
    lcd.clear();
}
```

#### displayOutdoorConditions()
```cpp
void displayOutdoorConditions(float humidity, float temperature) {
    lcd.setCursor(0, 0);
    lcd.print("Outdoor humidity:");
    lcd.setCursor(0, 1);
    lcd.print(humidity);
    lcd.print(" %");
    lcd.setCursor(0, 2);
    lcd.print("Outdoor temp: ");
    lcd.setCursor(0, 3);
    lcd.print(temperature);
    lcd.print(" *C");
}
```

#### displaySoilConditions()
```cpp
void displaySoilConditions(int moisture, float temperature) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil moisture: ");
    lcd.setCursor(0, 1);
    lcd.print(moisture);
    lcd.print("%");
    lcd.setCursor(0, 2);
    lcd.print("Soil temperature: ");
    lcd.setCursor(0, 3);
    lcd.print(temperature);
    lcd.print(" *C");
}
```

#### displayError()
```cpp
void displayError(String errorType) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(errorType + " error");
    lcd.setCursor(0, 1);
    lcd.print("Please check the");
    lcd.setCursor(0, 2);
    lcd.print("connection");
}
```

## Usage Examples

### Basic Setup Example
```cpp
#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <DHT.h>

// Initialize components
LiquidCrystal lcd(11,9,6,5,4,3);
DHT dht(8, DHT22);
OneWire oneWire(39);
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(9600);
    lcd.begin(20, 4);
    dht.begin();
    sensors.begin();
    
    // Configure pins
    pinMode(42, OUTPUT); // Watering pump
    pinMode(41, OUTPUT); // Draining pump
    pinMode(13, OUTPUT); // Status LED
    
    digitalWrite(13, HIGH); // Turn on status LED
}
```

### Reading Sensor Data Example
```cpp
void readSensors() {
    // Read air conditions
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    // Validate readings
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("DHT sensor error!");
        return;
    }
    
    // Read soil moisture
    int totalMoisture = 0;
    for (int i = 0; i < 5; i++) {
        totalMoisture += analogRead(A0 + i);
    }
    int avgMoisture = totalMoisture / 5;
    int moisturePercent = map(avgMoisture, 1023, 0, 0, 100);
    
    // Read soil temperature
    sensors.requestTemperatures();
    float soilTemp = sensors.getTempCByIndex(0);
    
    // Display results
    Serial.print("Air: ");
    Serial.print(temperature);
    Serial.print("°C, ");
    Serial.print(humidity);
    Serial.print("% | Soil: ");
    Serial.print(soilTemp);
    Serial.print("°C, ");
    Serial.print(moisturePercent);
    Serial.println("%");
}
```

### Custom Watering Control Example
```cpp
void customWateringControl(int moistureLevel) {
    const int LOW_MOISTURE = 60;
    const int HIGH_MOISTURE = 85;
    
    if (moistureLevel < LOW_MOISTURE) {
        // Start watering
        digitalWrite(42, HIGH);
        Serial.println("Watering started");
        
    } else if (moistureLevel > HIGH_MOISTURE) {
        // Start draining
        digitalWrite(41, HIGH);
        Serial.println("Draining started");
        
    } else {
        // Optimal range - stop all pumps
        digitalWrite(42, LOW);
        digitalWrite(41, LOW);
        Serial.println("Optimal moisture level");
    }
}
```

### Data Logging Example
```cpp
void logDataToSerial() {
    // Read all sensors
    float airTemp = dht.readTemperature();
    float airHumidity = dht.readHumidity();
    
    sensors.requestTemperatures();
    float soilTemp = sensors.getTempCByIndex(0);
    
    int soilMoisture = readAverageMoisture();
    
    // Format: timestamp,air_temp,air_humidity,soil_temp,soil_moisture
    Serial.print(millis());
    Serial.print(",");
    Serial.print(airTemp);
    Serial.print(",");
    Serial.print(airHumidity);
    Serial.print(",");
    Serial.print(soilTemp);
    Serial.print(",");
    Serial.println(soilMoisture);
}

int readAverageMoisture() {
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += analogRead(A0 + i);
    }
    return map(total / 5, 1023, 0, 0, 100);
}
```

## Troubleshooting

### Common Issues and Solutions

#### DHT Sensor Not Reading
**Symptoms**: NaN values, "DHT sensor error" message
**Solutions**:
- Check wiring connections to pin 8
- Verify 5V power supply
- Replace sensor if damaged
- Add pull-up resistor (10kΩ) if needed

#### DS18B20 Temperature Sensor Issues
**Symptoms**: DEVICE_DISCONNECTED_C error, "temp sensor error"
**Solutions**:
- Verify OneWire connection to pin 39
- Check 4.7kΩ pull-up resistor on data line
- Ensure proper power supply (3.3V or 5V)
- Test sensor with simple OneWire scan

#### Moisture Sensors Giving Wrong Readings
**Symptoms**: Inconsistent moisture percentages
**Solutions**:
- Calibrate wet/dry values for your soil type
- Clean sensor probes
- Check analog pin connections (A0-A4)
- Verify sensor power supply

#### Pumps Not Responding
**Symptoms**: No pump activation despite correct moisture levels
**Solutions**:
- Check relay connections and power supply
- Verify pump power requirements (12V)
- Test relays with multimeter
- Check digital pin outputs (41, 42)

#### LCD Display Issues
**Symptoms**: Blank screen, garbled text
**Solutions**:
- Verify all 6 pin connections (11,9,6,5,4,3)
- Check contrast adjustment (potentiometer)
- Ensure 5V power supply to LCD
- Test with simple "Hello World" program

### Error Codes and Messages

| Error Message | Cause | Solution |
|---------------|-------|----------|
| "DHT sensor error" | DHT22 disconnected or faulty | Check connections, replace sensor |
| "temp sensor error" | DS18B20 disconnected | Verify OneWire connection |
| "Failed to read from DHT sensor" | Communication failure | Check power and data connections |
| "Could not read temperature data" | DS18B20 communication error | Check pull-up resistor and wiring |

### Maintenance Schedule

#### Daily
- Check LCD display for error messages
- Verify pump operation sounds

#### Weekly
- Clean moisture sensor probes
- Check water reservoir levels
- Inspect all connections

#### Monthly
- Calibrate moisture sensors
- Test all sensors individually
- Clean LCD display
- Check relay operation

#### Seasonally
- Replace sensor batteries if applicable
- Deep clean all sensors
- Update calibration values for seasonal changes
- Backup configuration settings

## Configuration Parameters

### Moisture Thresholds
```cpp
// Adjustable parameters for different plant types
const int WATERING_START = 70;    // Start watering below this %
const int WATERING_STOP = 80;     // Stop watering above this %
const int DRAINAGE_START = 90;    // Start draining above this %
const int DRAINAGE_STOP = 85;     // Stop draining below this %
```

### Timing Parameters
```cpp
const int DISPLAY_DELAY = 2000;   // Display update interval (ms)
const int SENSOR_DELAY = 500;     // Sensor reading delay (ms)
const int BOOT_DELAY = 3000;      // Boot screen duration (ms)
```

### Calibration Values
```cpp
// Adjust these values based on your soil type and sensors
const int WET_SOIL_VALUE = 0;     // Analog value for wet soil
const int DRY_SOIL_VALUE = 1023;  // Analog value for dry soil
```

This documentation provides comprehensive coverage of all public APIs, functions, and components in the Smart Garden OS. Each section includes detailed explanations, parameter descriptions, return values, and practical usage examples to help users understand and implement the system effectively.