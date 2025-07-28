# Smart Garden OS - Quick Start Guide

## Prerequisites

### Hardware Requirements
- Arduino Mega 2560
- 20x4 LCD Display with I2C backpack (optional) or parallel connection
- DHT22 temperature and humidity sensor
- DS18B20 waterproof temperature sensor
- 5x Soil moisture sensors (analog)
- 2x 12V water pumps (watering and drainage)
- 2x 5V relay modules
- Jumper wires and breadboard/PCB
- 12V power supply for pumps
- 5V power supply for Arduino

### Software Requirements
- Arduino IDE (version 1.8.0 or later)
- Required libraries (see installation section)

## Installation

### 1. Install Arduino IDE
Download and install the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)

### 2. Install Required Libraries
Open Arduino IDE → Tools → Manage Libraries, then search and install:
- **DallasTemperature** by Miles Burton
- **OneWire** by Jim Studt  
- **DHT sensor library** by Adafruit
- **LiquidCrystal** (built-in, no installation needed)

### 3. Download Smart Garden OS
```bash
git clone https://github.com/your-username/SmartGardenOS.git
cd SmartGardenOS
```

## Hardware Setup

### Pin Connections

#### LCD Display (20x4)
| LCD Pin | Arduino Pin | Purpose |
|---------|-------------|---------|
| VSS | GND | Ground |
| VDD | 5V | Power |
| V0 | Potentiometer | Contrast |
| RS | 11 | Register Select |
| Enable | 9 | Enable |
| D4 | 6 | Data 4 |
| D5 | 5 | Data 5 |
| D6 | 4 | Data 6 |
| D7 | 3 | Data 7 |
| A | 5V | Backlight + |
| K | GND | Backlight - |

#### DHT22 Sensor
| DHT22 Pin | Arduino Pin | Purpose |
|-----------|-------------|---------|
| VCC | 5V | Power |
| Data | 8 | Data signal |
| GND | GND | Ground |

#### DS18B20 Temperature Sensor
| DS18B20 Pin | Arduino Pin | Purpose |
|-------------|-------------|---------|
| Red (VCC) | 5V | Power |
| Yellow (Data) | 39 | Data signal |
| Black (GND) | GND | Ground |

**Note**: Add a 4.7kΩ pull-up resistor between Data (pin 39) and VCC (5V)

#### Soil Moisture Sensors
| Sensor | Arduino Pin | Purpose |
|--------|-------------|---------|
| Sensor 1 | A0 | Analog reading |
| Sensor 2 | A1 | Analog reading |
| Sensor 3 | A2 | Analog reading |
| Sensor 4 | A3 | Analog reading |
| Sensor 5 | A4 | Analog reading |

Connect VCC to 5V and GND to ground for all sensors.

#### Relay Modules and Pumps
| Component | Arduino Pin | Purpose |
|-----------|-------------|---------|
| Watering Relay | 42 | Control watering pump |
| Drainage Relay | 41 | Control drainage pump |
| Status LED | 13 | System status |

**Relay Wiring**:
- VCC → 5V
- GND → GND  
- IN1 (Watering) → Pin 42
- IN2 (Drainage) → Pin 41

**Pump Wiring**:
- Connect pumps to relay COM and NO terminals
- Use 12V power supply for pumps
- Ensure proper polarity

## Software Setup

### 1. Open the Code
1. Launch Arduino IDE
2. Open `SmartGardenOS.ino`

### 2. Configure Settings (Optional)
Modify these values in the code if needed:

```cpp
// Moisture thresholds (adjust for your plants)
const int WATERING_THRESHOLD = 70;  // Start watering below 70%
const int DRAINAGE_THRESHOLD = 90;  // Start draining above 90%

// Sensor calibration (adjust for your soil type)
const int wet = 0;      // Wet soil value
const int dry = 1023;   // Dry soil value
```

### 3. Upload to Arduino
1. Connect Arduino Mega via USB
2. Select **Tools → Board → Arduino Mega or Mega 2560**
3. Select **Tools → Port** → (your Arduino's port)
4. Click **Upload** button

## First Run

### 1. Power On
1. Connect 12V power supply for pumps
2. Connect Arduino to USB or 5V power supply
3. System will display boot screen for 3 seconds

### 2. Verify Sensors
Watch the LCD display cycle through:
- **Outdoor conditions**: Air temperature and humidity
- **Soil conditions**: Soil moisture percentage and temperature

### 3. Check Serial Monitor
1. Open **Tools → Serial Monitor**
2. Set baud rate to **9600**
3. You should see sensor readings printed continuously

Expected output:
```
Humidity: 45.2 %, Outdoor temperature: 23.5 *C,  
Soil moisture: 65 %, Soil temperature: 22.1 C,
```

## Testing the System

### 1. Test Sensors Individually

#### DHT22 Test
- Breathe on sensor → humidity should increase
- If showing "DHT sensor error" → check connections

#### DS18B20 Test  
- Touch sensor → temperature should change slightly
- If showing "temp sensor error" → check wiring and pull-up resistor

#### Moisture Sensors Test
- Insert sensors in dry soil → should show low percentage
- Water the soil → percentage should increase
- If readings seem wrong → calibrate wet/dry values

### 2. Test Pump Control

#### Manual Test (Optional)
Add this temporary code to test pumps:
```cpp
// Add to loop() for testing
digitalWrite(42, HIGH); // Turn on watering pump
delay(2000);
digitalWrite(42, LOW);  // Turn off watering pump
delay(1000);
digitalWrite(41, HIGH); // Turn on drainage pump  
delay(2000);
digitalWrite(41, LOW);  // Turn off drainage pump
delay(5000);
```

#### Automatic Test
- Place sensors in dry soil (< 70% moisture)
- Watering pump should activate
- Add water until moisture > 80%
- Watering pump should stop

## Calibration

### Moisture Sensor Calibration
1. **Dry Calibration**:
   - Remove sensors from soil
   - Note the analog values (should be near 1023)
   - Update `const int dry = 1023;` if needed

2. **Wet Calibration**:
   - Submerge sensors in water
   - Note the analog values (should be near 0)
   - Update `const int wet = 0;` if needed

### Threshold Adjustment
Modify thresholds based on your plants:
```cpp
// For succulents (prefer drier soil)
const int WATERING_THRESHOLD = 40;
const int DRAINAGE_THRESHOLD = 70;

// For tropical plants (prefer moist soil)  
const int WATERING_THRESHOLD = 80;
const int DRAINAGE_THRESHOLD = 95;
```

## Troubleshooting

### Common Issues

#### "DHT sensor error"
- **Check connections**: Ensure DHT22 data pin is connected to pin 8
- **Power supply**: Verify 5V and GND connections
- **Sensor orientation**: Ensure correct pin identification

#### "temp sensor error"  
- **Pull-up resistor**: Add 4.7kΩ resistor between data pin (39) and 5V
- **Connections**: Verify OneWire connection to pin 39
- **Sensor polarity**: Check red=5V, black=GND, yellow=data

#### LCD Display Issues
- **Blank screen**: Check all 6 data connections and power
- **Garbled text**: Adjust contrast potentiometer
- **No backlight**: Check backlight power connections

#### Pumps Not Working
- **Relay LEDs**: Should light up when activated
- **Power supply**: Ensure 12V supply for pumps
- **Connections**: Verify relay control pins (41, 42)

#### Inconsistent Moisture Readings
- **Sensor placement**: Ensure sensors are properly inserted in soil
- **Calibration**: Recalibrate wet/dry values for your soil type
- **Cleaning**: Clean sensor probes regularly

### Getting Help
- Check the full **API_DOCUMENTATION.md** for detailed troubleshooting
- Open an issue on GitHub with:
  - Error messages from Serial Monitor
  - Photos of your wiring setup
  - Description of the problem

## Next Steps

### Data Logging
Connect to a computer and log data to CSV:
```cpp
// Add to loop() for CSV logging
Serial.print(millis()); Serial.print(",");
Serial.print(dht.readTemperature()); Serial.print(",");
Serial.print(dht.readHumidity()); Serial.print(",");
Serial.print(sensors.getTempCByIndex(0)); Serial.print(",");
Serial.println(hygroVal);
```

### Remote Monitoring
- Add WiFi module (ESP32/ESP8266) for remote monitoring
- Implement web dashboard for data visualization
- Set up alerts for sensor failures

### Advanced Features
- Add pH sensors for soil analysis
- Implement fertilizer dispensing system
- Add light sensors for optimal placement
- Create mobile app for remote control

## Safety Notes

⚠️ **Important Safety Information**:
- Never leave pumps running unattended for extended periods
- Use appropriate fuses for 12V pump circuits
- Keep electrical connections away from water
- Regularly inspect all connections for corrosion
- Use waterproof enclosures for outdoor installations

## Maintenance Schedule

### Weekly
- Check water reservoir levels
- Clean moisture sensor probes
- Verify pump operation

### Monthly  
- Test all sensors individually
- Calibrate moisture readings
- Check all electrical connections
- Clean LCD display

### Seasonally
- Deep clean all sensors
- Update calibration for seasonal changes
- Inspect pumps and tubing for wear
- Test emergency shutoff procedures

---

**Congratulations!** Your Smart Garden OS should now be running. The system will automatically monitor your plants and water them as needed. Check the API documentation for advanced customization options.