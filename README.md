# Smart Garden OS

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue.svg)](https://www.arduino.cc/)
[![Version](https://img.shields.io/badge/Version-1.0-green.svg)](https://github.com/HugoQuintart/SmartGardenOS)

Smart Garden OS is an open-source Arduino-based automated gardening system that intelligently monitors and maintains optimal growing conditions for plants. The system features comprehensive environmental monitoring and automated watering with drainage capabilities.

## 🌱 Features

- **Multi-Sensor Monitoring**: Tracks soil moisture, soil temperature, air temperature, and humidity
- **Intelligent Watering**: Automatically waters plants based on soil moisture levels
- **Drainage System**: Prevents overwatering with automatic excess water removal
- **Real-Time Display**: 20x4 LCD showing live sensor readings and system status
- **Data Logging**: Serial output for data analysis and graphing
- **Fail-Safe Operation**: Error detection and handling for all sensors
- **Customizable Thresholds**: Adjustable watering parameters for different plant types

## 📋 Table of Contents

- [Quick Start](#quick-start)
- [Documentation](#documentation)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

## 🚀 Quick Start

1. **Hardware Setup**: Connect all components according to the [Component Reference Guide](COMPONENT_REFERENCE.md)
2. **Software Setup**: Install required libraries and upload the code
3. **Calibration**: Calibrate moisture sensors for your soil type
4. **Testing**: Verify all sensors and pump operations

For detailed instructions, see the [Quick Start Guide](QUICK_START_GUIDE.md).

## 📚 Documentation

This project includes comprehensive documentation:

- **[API Documentation](API_DOCUMENTATION.md)** - Complete API reference for all functions and components
- **[Quick Start Guide](QUICK_START_GUIDE.md)** - Step-by-step setup instructions
- **[Component Reference](COMPONENT_REFERENCE.md)** - Detailed hardware component specifications
- **[Hardware List](HardwareList.txt)** - Complete parts list with purchase links

## 🔧 Hardware Requirements

### Core Components
- Arduino Mega 2560
- 20x4 LCD Display
- DHT22 Temperature/Humidity Sensor
- DS18B20 Waterproof Temperature Sensor
- 5× Soil Moisture Sensors
- 2× 12V Water Pumps
- 2× 5V Relay Modules

### Power & Connectivity
- 12V Power Supply (5A)
- 5V Power Supply (2A) or USB
- Jumper wires and connectors
- 4.7kΩ Pull-up resistor
- 10kΩ Potentiometer (LCD contrast)

See [HardwareList.txt](HardwareList.txt) for specific product recommendations and purchase links.

## 💻 Installation

### Prerequisites
- Arduino IDE (1.8.0 or later)
- USB cable for Arduino programming

### Required Libraries
Install these libraries through Arduino IDE Library Manager:
```
- DallasTemperature by Miles Burton
- OneWire by Jim Studt
- DHT sensor library by Adafruit
- LiquidCrystal (built-in)
```

### Upload Code
1. Clone this repository
2. Open `SmartGardenOS.ino` in Arduino IDE
3. Select "Arduino Mega or Mega 2560" board
4. Select appropriate COM port
5. Upload the code

## 🎯 Usage

### Basic Operation
The system automatically:
1. Reads all sensors every 2 seconds
2. Displays readings on LCD screen
3. Controls pumps based on soil moisture:
   - **< 70%**: Start watering
   - **> 90%**: Start draining
   - **71-80%**: Optimal range (pumps off)

### Serial Monitor
Connect to serial monitor (9600 baud) to view:
- Real-time sensor readings
- System status messages
- Error notifications
- Data suitable for CSV logging

### Display Screens
The LCD alternates between two screens:
- **Screen 1**: Outdoor temperature and humidity
- **Screen 2**: Soil moisture and temperature

## ⚙️ Configuration

### Moisture Thresholds
Adjust these values in the code for different plant types:

```cpp
// Current settings (general purpose)
const int WATERING_THRESHOLD = 70;  // Start watering below 70%
const int DRAINAGE_THRESHOLD = 90;  // Start draining above 90%

// For succulents (prefer drier soil)
const int WATERING_THRESHOLD = 40;
const int DRAINAGE_THRESHOLD = 70;

// For tropical plants (prefer moist soil)
const int WATERING_THRESHOLD = 80;
const int DRAINAGE_THRESHOLD = 95;
```

### Sensor Calibration
Calibrate moisture sensors for your soil type:

```cpp
// Adjust these values based on your sensors and soil
const int wet = 0;      // Value when sensors are in water
const int dry = 1023;   // Value when sensors are in air
```

## 🏗️ DIY Construction

### 3D Printing
- 3D files are available in the `3D.zip` archive
- Recommended material: PLA plastic
- Printer recommendation: Wanhao Duplicator 12/230 or similar

### Assembly Tips
- Use weatherproof enclosure for outdoor installation
- Ensure proper cable management and strain relief
- Test all connections before final assembly
- Consider adding fuses for safety

## 🔍 Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| "DHT sensor error" | Check connections to pin 8, verify power supply |
| "temp sensor error" | Add 4.7kΩ pull-up resistor, check OneWire connection |
| LCD blank/garbled | Verify all pin connections, adjust contrast |
| Pumps not working | Check relay connections and 12V power supply |
| Inconsistent moisture readings | Clean sensors, recalibrate wet/dry values |

For detailed troubleshooting, see the [API Documentation](API_DOCUMENTATION.md#troubleshooting).

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Areas for Contribution
- Additional sensor support
- Web interface development
- Mobile app integration
- Enhanced error handling
- Power optimization
- Documentation improvements

## 📊 Data Analysis

The system outputs CSV-compatible data via serial connection:
```
timestamp,air_temp,air_humidity,soil_temp,soil_moisture
12500,23.5,45.2,22.1,65
```

This data can be imported into Excel, Google Sheets, or other analysis tools for:
- Growth tracking
- Environmental correlation analysis
- System performance monitoring
- Predictive watering schedules

## 🛡️ Safety Notes

⚠️ **Important Safety Information**:
- Never leave pumps running unattended for extended periods
- Use appropriate fuses for all power circuits
- Keep electrical connections away from water
- Use weatherproof enclosures for outdoor installations
- Regularly inspect all connections for corrosion

## 📞 Support

If you have questions or need help:

- Check the comprehensive [documentation](API_DOCUMENTATION.md)
- Review the [troubleshooting guide](API_DOCUMENTATION.md#troubleshooting)
- Open an issue on GitHub with detailed information
- Email: hugo.quintart@athenee-ath.be

## 🌍 Project History

This project has been in development since November 2020, evolving from a simple watering system to a comprehensive garden monitoring solution. The system has been tested in various environmental conditions and continues to be improved based on user feedback.

## 📷 Gallery

![Smart Garden OS Overview](https://user-images.githubusercontent.com/98947898/233804508-c17f80a4-8970-455f-a51a-f2bc423157d2.png)
*Complete system overview showing all components*

![System in Operation](https://user-images.githubusercontent.com/98947898/233804519-318945fd-b74b-4350-8226-31cd4855491d.png)
*LCD display showing real-time sensor readings*

## 📄 License

This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. 

**What this means**:
- ✅ You can use, modify, and share this project
- ✅ You can build your own Smart Garden OS
- ✅ You can contribute improvements back to the community
- ❌ You cannot sell this project or make commercial profit from it
- ❌ You cannot use this for commercial purposes

This project was made with love and is shared freely with the maker community. Please respect the license terms and help keep this project open and accessible to everyone.

---

**Made with ❤️ by Hugo Quintart**  
*"Growing plants smarter, one sensor at a time"*
