# Smart Study Environment Monitoring & Analysis System

An ESP8266-based embedded system that monitors a student's study environment, detects presence, tracks study and break time, calculates an environment score, and provides useful recommendations.

## 📌 Project Overview

The Smart Study Environment Monitoring & Analysis System is an embedded project developed using the ESP8266 NodeMCU.

The system collects data from multiple sensors and processes it to provide information about the study environment.

The system monitors:

- 🌡️ Temperature
- 💧 Humidity
- 💡 Light level
- 📏 Person presence using distance measurement
- ⏱️ Study time
- ☕ Break time
- 📊 Environment score
- 💡 Low-light indication

The results are displayed on a 16×2 I2C LCD and through the Serial Monitor.

## 🎯 Objectives

- Monitor temperature and humidity using DHT11.
- Measure light level using an LDR.
- Detect nearby person presence using HC-SR04.
- Track study and break time automatically.
- Calculate an environment score out of 100.
- Control an LED when the light level is low.
- Display system information on an I2C LCD.
- Provide study recommendations and efficiency ratings.

## 🛠️ Components Used

| Component | Purpose |
|---|---|
| ESP8266 NodeMCU | Main controller |
| DHT11 | Temperature and humidity measurement |
| LDR | Light-level measurement |
| HC-SR04 | Distance and presence detection |
| 16×2 I2C LCD | Display |
| LED | Low-light indication |
| Resistor | LED current limiting |
| Breadboard & jumper wires | Connections |

## 🔌 Pin Connections

| Component | ESP8266 Pin |
|---|---|
| DHT11 DATA | D4 |
| HC-SR04 TRIG | D5 |
| HC-SR04 ECHO | D6 |
| LED | D7 |
| LDR | A0 |
| LCD SDA | D2 |
| LCD SCL | D1 |

> **Note:** A typical 5 V HC-SR04 ECHO signal should be appropriately level-shifted/voltage-divided before connecting it to an ESP8266 GPIO.

## ⚙️ Working

The sensors continuously provide environmental information to the ESP8266.

The ESP8266:

1. Reads temperature and humidity from the DHT11.
2. Reads the analog light level from the LDR.
3. Measures distance using the HC-SR04.
4. Determines whether a person is present.
5. Calculates an environment score.
6. Tracks study and absence time.
7. Controls the LED based on light level.
8. Displays information on the LCD.
9. Sends detailed information to the Serial Monitor.

### System Data Flow

**Sensors → ESP8266 → Data Processing → Score & Status → LCD / LED / Serial Monitor**

## 📊 Environment Score

The system calculates a score out of 100 based on:

- Temperature: 25 points
- Humidity: 25 points
- Light: 25 points
- Person presence: 25 points

The maximum score is **100**.

## ⏱️ Study and Break Detection

When a person is detected within the specified distance:

- Study time increases.
- Absence time is reset.

When the person is not detected:

- Break/absence time increases.
- After 300 seconds of continuous absence, a break is counted.

The system uses `millis()` for elapsed-time measurement.

## 💡 Low-Light Detection

The LED uses hysteresis to avoid rapid ON/OFF switching:

- Light below 150 → LED ON
- Light above 250 → LED OFF
- Between 150 and 250 → previous LED state is maintained

## 🖥️ LCD Display

The LCD cycles through multiple pages showing:

1. Temperature, humidity and light
2. Distance and person status
3. Study time
4. Environment score and light status
5. Current status and recommendation
6. Efficiency rating

## 💻 Technologies Used

- Embedded C/C++
- ESP8266 NodeMCU
- Arduino IDE
- GPIO
- Analog input
- Sensor interfacing
- I2C communication
- Serial communication
- `millis()` based timing

## 📁 Repository Contents

- `Smart_Study_Environment.ino` — Complete project source code
- `Smart_Study_Project_Report.pdf` — Complete project report
- `.gitignore` — Git ignore configuration
- `LICENSE` — Project license

## 🚀 Future Scope

Possible improvements include:

- Wi-Fi/cloud data logging
- Web or mobile dashboard
- Daily and weekly study graphs
- RTC-based date and time tracking
- More accurate environmental sensors
- Personalized study analytics
- Automated lighting or fan control

## 👩‍💻 Project

**Smart Study Environment Monitoring & Analysis System**

Developed as an embedded systems project using ESP8266 NodeMCU.
