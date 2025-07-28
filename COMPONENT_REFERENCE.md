# Smart Garden OS - Component Reference Guide

## Table of Contents
- [Arduino Mega 2560](#arduino-mega-2560)
- [LCD Display (20x4)](#lcd-display-20x4)
- [DHT22 Sensor](#dht22-sensor)
- [DS18B20 Temperature Sensor](#ds18b20-temperature-sensor)
- [Soil Moisture Sensors](#soil-moisture-sensors)
- [Relay Modules](#relay-modules)
- [Water Pumps](#water-pumps)
- [Power Supply](#power-supply)
- [Additional Components](#additional-components)

## Arduino Mega 2560

### Overview
The Arduino Mega 2560 serves as the main microcontroller for the Smart Garden OS. It provides sufficient I/O pins and memory for handling multiple sensors and actuators.

### Specifications
- **Microcontroller**: ATmega2560
- **Operating Voltage**: 5V
- **Input Voltage**: 7-12V (recommended)
- **Digital I/O Pins**: 54 (15 provide PWM output)
- **Analog Input Pins**: 16
- **Flash Memory**: 256 KB
- **SRAM**: 8 KB
- **EEPROM**: 4 KB
- **Clock Speed**: 16 MHz

### Pin Usage in Smart Garden OS
```cpp
// Digital Pins Used
Pin 3  - LCD D7
Pin 4  - LCD D6  
Pin 5  - LCD D5
Pin 6  - LCD D4
Pin 8  - DHT22 Data
Pin 9  - LCD Enable
Pin 11 - LCD RS
Pin 13 - Status LED
Pin 39 - DS18B20 Data
Pin 41 - Drainage Pump Relay
Pin 42 - Watering Pump Relay

// Analog Pins Used
Pin A0 - Soil Moisture Sensor 1
Pin A1 - Soil Moisture Sensor 2
Pin A2 - Soil Moisture Sensor 3
Pin A3 - Soil Moisture Sensor 4
Pin A4 - Soil Moisture Sensor 5
```

### Power Considerations
- Can be powered via USB (5V) or external adapter (7-12V)
- USB power is sufficient for development/testing
- External power recommended for production use with pumps
- Built-in voltage regulator provides 5V and 3.3V outputs

### Programming
- Uses Arduino IDE with ATmega2560 board selection
- Upload via USB cable (USB-B connector)
- Bootloader allows easy program updates
- Serial communication at 9600 baud for debugging

## LCD Display (20x4)

### Overview
The 20x4 character LCD displays real-time sensor readings and system status. It uses the standard HD44780 controller for easy interfacing.

### Specifications
- **Display Size**: 20 characters × 4 lines
- **Controller**: HD44780 compatible
- **Interface**: 4-bit or 8-bit parallel (using 4-bit mode)
- **Operating Voltage**: 5V
- **Backlight**: LED with adjustable brightness
- **Character Set**: ASCII + Japanese characters

### Pin Configuration
```cpp
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);
//                RS EN D4 D5 D6 D7
```

### Connection Details
| LCD Pin | Function | Arduino Pin | Description |
|---------|----------|-------------|-------------|
| 1 (VSS) | Ground | GND | Power ground |
| 2 (VDD) | Power | 5V | +5V power supply |
| 3 (V0) | Contrast | Potentiometer | Contrast adjustment |
| 4 (RS) | Register Select | 11 | Command/Data selection |
| 5 (R/W) | Read/Write | GND | Always write mode |
| 6 (E) | Enable | 9 | Enable signal |
| 7-10 | D0-D3 | Not connected | Lower data bits (unused in 4-bit mode) |
| 11-14 | D4-D7 | 6,5,4,3 | Upper data bits |
| 15 (A) | Backlight + | 5V | Backlight power |
| 16 (K) | Backlight - | GND | Backlight ground |

### Display Layout
```
Line 0: [System Status/Sensor Type]
Line 1: [Primary Value          ]
Line 2: [Secondary Info         ]
Line 3: [Additional Data        ]
```

### Usage Examples
```cpp
// Initialize display
lcd.begin(20, 4);

// Clear and position cursor
lcd.clear();
lcd.setCursor(0, 0);  // Column 0, Row 0

// Display text
lcd.print("Smart Garden OS");

// Display variables
lcd.print(temperature);
lcd.print(" *C");
```

### Troubleshooting
- **Blank screen**: Check power connections and contrast
- **Garbled text**: Verify data pin connections
- **No backlight**: Check backlight power connections
- **Dim display**: Adjust contrast potentiometer

## DHT22 Sensor

### Overview
The DHT22 (AM2302) is a digital temperature and humidity sensor providing calibrated digital output. It's more accurate than the DHT11 and suitable for outdoor monitoring.

### Specifications
- **Temperature Range**: -40°C to +80°C (±0.5°C accuracy)
- **Humidity Range**: 0-100% RH (±2-5% accuracy)
- **Resolution**: 0.1°C temperature, 0.1% humidity
- **Sampling Rate**: 0.5 Hz (one reading every 2 seconds)
- **Operating Voltage**: 3.3V - 6V
- **Current**: 1-1.5mA during measurement

### Pin Configuration
| Pin | Function | Connection |
|-----|----------|------------|
| 1 | VCC | 5V |
| 2 | Data | Pin 8 |
| 3 | NULL | Not connected |
| 4 | GND | Ground |

### Code Implementation
```cpp
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Check for errors
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT sensor error!");
    return;
  }
  
  // Calculate heat index
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
}
```

### Calibration
- Factory calibrated - no user calibration needed
- Cross-reference with known accurate thermometer/hygrometer
- Allow 2-second minimum between readings
- Sensor needs 1-2 seconds to stabilize after power-on

### Environmental Considerations
- Protect from direct sunlight for accurate readings
- Ensure adequate ventilation around sensor
- Avoid placing near heat sources
- Consider weatherproof housing for outdoor use

## DS18B20 Temperature Sensor

### Overview
The DS18B20 is a waterproof digital temperature sensor ideal for soil temperature monitoring. It uses the 1-Wire protocol for communication and can operate over long cable distances.

### Specifications
- **Temperature Range**: -55°C to +125°C
- **Accuracy**: ±0.5°C from -10°C to +85°C
- **Resolution**: 9-12 bits (configurable)
- **Conversion Time**: 750ms (12-bit resolution)
- **Operating Voltage**: 3.0V - 5.5V
- **Interface**: 1-Wire digital

### Pin Configuration
| Wire Color | Function | Connection |
|------------|----------|------------|
| Red | VCC | 5V |
| Black | GND | Ground |
| Yellow/White | Data | Pin 39 |

**Important**: Add 4.7kΩ pull-up resistor between Data and VCC

### Code Implementation
```cpp
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 39
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");
  } else {
    Serial.println("Temperature sensor error!");
  }
}
```

### Multiple Sensor Support
```cpp
// Get number of connected sensors
int sensorCount = sensors.getDeviceCount();

// Read specific sensor by index
float temp1 = sensors.getTempCByIndex(0);
float temp2 = sensors.getTempCByIndex(1);

// Read sensor by address (more reliable)
DeviceAddress sensor1Address;
sensors.getAddress(sensor1Address, 0);
float temp = sensors.getTempC(sensor1Address);
```

### Installation Tips
- Bury sensor 2-4 inches deep in soil
- Ensure waterproof seal is intact
- Keep cable connections dry
- Use cable with sufficient gauge for long runs
- Test continuity before installation

## Soil Moisture Sensors

### Overview
Capacitive soil moisture sensors measure soil moisture content by detecting changes in the dielectric constant of the soil. They're more durable than resistive sensors and don't corrode over time.

### Specifications
- **Operating Voltage**: 3.3V - 5V
- **Output**: 0-3V analog (proportional to moisture)
- **Response Time**: < 1 second
- **Probe Material**: PCB with protective coating
- **Operating Temperature**: -40°C to +85°C

### Sensor Placement
```cpp
// 5 sensors for comprehensive coverage
int moisturePin1 = A0;  // Zone 1
int moisturePin2 = A1;  // Zone 2  
int moisturePin3 = A2;  // Zone 3
int moisturePin4 = A3;  // Zone 4
int moisturePin5 = A4;  // Zone 5
```

### Reading and Processing
```cpp
// Read all sensors
int totalMoisture = analogRead(A0) + analogRead(A1) + 
                   analogRead(A2) + analogRead(A3) + 
                   analogRead(A4);

// Calculate average
int avgMoisture = totalMoisture / 5;

// Convert to percentage
int moisturePercent = map(avgMoisture, dry, wet, 0, 100);
```

### Calibration Process
1. **Dry Calibration**:
   ```cpp
   // Remove from soil, read value
   int dryValue = analogRead(A0);  // Typically ~1023
   const int dry = dryValue;
   ```

2. **Wet Calibration**:
   ```cpp
   // Submerge in water, read value
   int wetValue = analogRead(A0);  // Typically ~300-500
   const int wet = wetValue;
   ```

3. **Soil-Specific Calibration**:
   - Test in actual growing medium
   - Different soil types have different ranges
   - Clay soil: Different range than sandy soil
   - Adjust wet/dry values accordingly

### Maintenance
- Clean probes monthly to remove soil buildup
- Check for corrosion on connections
- Verify consistent readings across all sensors
- Replace if readings become erratic

## Relay Modules

### Overview
5V relay modules provide electrical isolation and switching capability for controlling 12V water pumps. Each module contains an optocoupler for safe operation.

### Specifications
- **Coil Voltage**: 5V DC
- **Contact Rating**: 10A @ 250V AC, 10A @ 30V DC
- **Isolation**: Optocoupler isolation
- **Trigger**: Low-level trigger (active LOW)
- **LED Indicators**: Power and relay status
- **Dimensions**: ~50mm × 26mm × 18mm

### Connection Details
```cpp
// Relay control pins
#define WATERING_PIN 42  // Controls watering pump
#define DRAINING_PIN 41  // Controls drainage pump

// Relay module connections
VCC  → 5V
GND  → Ground
IN1  → Pin 42 (Watering)
IN2  → Pin 41 (Drainage)
```

### Load Connections
```
Relay 1 (Watering):
COM  → +12V supply
NO   → Watering pump positive
NC   → Not connected

Relay 2 (Drainage):
COM  → +12V supply  
NO   → Drainage pump positive
NC   → Not connected

Both pump negatives → 12V supply negative
```

### Control Code
```cpp
void setup() {
  pinMode(WATERING_PIN, OUTPUT);
  pinMode(DRAINING_PIN, OUTPUT);
  
  // Initialize relays OFF (HIGH = OFF for low-trigger relays)
  digitalWrite(WATERING_PIN, HIGH);
  digitalWrite(DRAINING_PIN, HIGH);
}

// Turn on watering pump
void startWatering() {
  digitalWrite(WATERING_PIN, LOW);  // LOW = ON
}

// Turn off watering pump
void stopWatering() {
  digitalWrite(WATERING_PIN, HIGH); // HIGH = OFF
}
```

### Safety Features
- Optocoupler isolation protects Arduino
- LED indicators show relay status
- Flyback diodes protect from inductive loads
- Screw terminals for secure connections

### Troubleshooting
- **Relay not clicking**: Check 5V power and control signal
- **LED on but no switching**: Faulty relay contacts
- **Pump runs continuously**: Check control logic and wiring
- **Intermittent operation**: Loose connections or insufficient power

## Water Pumps

### Overview
12V DC submersible pumps provide water circulation for the irrigation and drainage systems. They're designed for continuous operation and can handle small particles.

### Specifications
- **Operating Voltage**: 12V DC
- **Current Draw**: 0.5-2A (depending on load)
- **Flow Rate**: 1-3 L/min (varies by model)
- **Head Pressure**: 1-3 meters
- **Inlet**: 6mm barb fitting
- **Outlet**: 6mm barb fitting
- **Duty Cycle**: Continuous operation capable

### Installation
```
Water Source → Pump Inlet → Pump → Outlet Tubing → Distribution Points

Drainage:
Collection Points → Drainage Tubing → Pump Inlet → Pump → Waste Container
```

### Electrical Connections
```cpp
// Through relay modules
12V Supply (+) → Relay COM terminals
Relay NO terminals → Pump positive wires
Pump negative wires → 12V Supply (-)

// Control from Arduino
digitalWrite(WATERING_PIN, LOW);  // Start watering pump
digitalWrite(DRAINING_PIN, LOW);  // Start drainage pump
```

### Pump Control Logic
```cpp
void controlPumps(int moistureLevel) {
  if (moistureLevel <= 70) {
    // Soil too dry - start watering
    digitalWrite(WATERING_PIN, LOW);
    digitalWrite(DRAINING_PIN, HIGH);
    
  } else if (moistureLevel >= 90) {
    // Soil too wet - start draining
    digitalWrite(WATERING_PIN, HIGH);
    digitalWrite(DRAINING_PIN, LOW);
    
  } else if (moistureLevel >= 71 && moistureLevel <= 80) {
    // Optimal range - stop both pumps
    digitalWrite(WATERING_PIN, HIGH);
    digitalWrite(DRAINING_PIN, HIGH);
  }
}
```

### Maintenance
- **Daily**: Check for unusual noises or vibrations
- **Weekly**: Verify flow rates and check for clogs
- **Monthly**: Clean intake filters and inspect tubing
- **Seasonally**: Full disassembly and cleaning

### Troubleshooting
- **No flow**: Check power, prime pump, clear blockages
- **Reduced flow**: Clean filters, check for air leaks
- **Noisy operation**: Check for cavitation or worn bearings
- **Overheating**: Reduce duty cycle, check for blockages

## Power Supply

### Overview
The system requires two power supplies: 5V for Arduino and sensors, and 12V for the water pumps. Proper power distribution ensures reliable operation.

### Power Requirements
```
5V Rail:
- Arduino Mega: ~50mA
- LCD Display: ~20mA (without backlight)
- LCD Backlight: ~150mA
- DHT22: ~1.5mA
- DS18B20: ~1mA
- Moisture Sensors: ~5mA each
- Relay Modules: ~20mA each
Total 5V: ~300mA (add 50% margin = 450mA)

12V Rail:
- Water Pumps: 0.5-2A each
- Total 12V: 4A maximum (both pumps running)
```

### Recommended Supplies
- **5V**: USB power bank (2A capacity) or 5V/2A wall adapter
- **12V**: 12V/5A switching power supply
- **Alternative**: Single 12V supply with 5V buck converter

### Power Distribution
```
12V Supply → Relay COM terminals → Pumps
12V Supply → Buck Converter → 5V Rail → Arduino/Sensors

OR

5V USB Supply → Arduino VIN
12V Supply → Relay COM terminals → Pumps
```

### Protection
- Add fuses: 5A for 12V rail, 1A for 5V rail
- Use proper gauge wire for current capacity
- Ensure good connections to prevent voltage drops
- Consider surge protection for outdoor installations

## Additional Components

### Pull-up Resistors
- **DS18B20**: 4.7kΩ between data line and VCC
- **Purpose**: Ensures proper digital signal levels
- **Installation**: Solder between pins 39 and 5V

### Contrast Potentiometer (LCD)
- **Value**: 10kΩ linear potentiometer
- **Connection**: Outer pins to 5V and GND, center to LCD V0
- **Purpose**: Adjusts LCD contrast for optimal visibility

### Status LED
- **Built-in**: Arduino Mega has LED on pin 13
- **External**: Add LED + 220Ω resistor if needed
- **Purpose**: Visual indication of system operation

### Connectors and Wiring
- **Screw terminals**: For pump connections
- **Dupont connectors**: For sensor connections
- **Wire gauge**: 22-24 AWG for signals, 16-18 AWG for pumps
- **Heat shrink tubing**: For connection protection

### Enclosure Considerations
- **Weatherproof rating**: IP65 or higher for outdoor use
- **Ventilation**: Prevent condensation buildup
- **Access ports**: For cables and maintenance
- **Mounting**: Secure mounting for stability

### Tools Required
- Soldering iron and solder
- Wire strippers
- Multimeter
- Screwdrivers (Phillips and flathead)
- Heat gun (for heat shrink tubing)
- Drill and bits (for enclosure mounting)

This component reference provides detailed information about each hardware component used in the Smart Garden OS, including specifications, connections, code examples, and troubleshooting guidance.