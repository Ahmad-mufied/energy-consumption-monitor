# Energy Consumption Monitor (ECM)

This repository contains the code and resources for the Energy Consumption Monitor (ECM) project. The ECM is a system designed to monitor and record energy usage using a PZEM-004T sensor module and upload the data to an InfluxDB database for analysis.

## Table of Contents

- [Energy Consumption Monitor (ECM)](#energy-consumption-monitor-ecm)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Hardware Components](#hardware-components)
  - [Software and Services](#software-and-services)
  - [Libraries Used](#libraries-used)
  - [Setup and Installation](#setup-and-installation)
  - [Usage](#usage)
  - [Dashboard](#dashboard)
  - [Images](#images)

## Introduction

The Energy Consumption Monitor (ECM) is developed to help users monitor their energy usage in real-time. It uses a PZEM-004T sensor connected to an ESP8266 or ESP32 microcontroller, and uploads the energy data to an InfluxDB instance. The data can then be visualized and analyzed using tools such as Grafana.

## Features

- Real-time monitoring of voltage, current, power, energy, frequency, and power factor.
- Data upload to InfluxDB for persistent storage and analysis.
- Compatibility with ESP8266 and ESP32 microcontrollers.
- Easy setup and integration with existing energy monitoring systems.

## Hardware Components

- **ESP8266 or ESP32 microcontroller**: This project supports both, but the ESP8266 is preferred due to the use of SoftwareSerial.
- **PZEM-004T v3.0**: Energy monitoring sensor.
- **Jumper cables**
- **Power plug**
- **Power socket**

## Software and Services

- Arduino IDE
- InfluxDB
- Grafana
- Telegram Bot API

## Libraries Used

This project uses the following Arduino libraries:

- **[PZEM-004T v3.0 Library](https://github.com/olehs/PZEM004Tv30)**: For interfacing with the PZEM-004T energy monitor.
- **[SoftwareSerial](https://www.arduino.cc/en/Reference/softwareSerial)**: For serial communication (only for ESP8266).
- **[InfluxDB Client for Arduino](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)**: For sending data to InfluxDB.

## Setup and Installation

1. Connect the PZEM-004T sensor to the Wemos D1 R2 using jumper cables according to the provided circuit diagram.
2. Install the Arduino IDE and necessary libraries:
   - ESP8266WiFi
   - InfluxDbClient
   - PZEM004Tv30
3. Set up an InfluxDB instance for data storage.
4. Configure a Grafana server and create dashboards for data visualization.
5. Create a Telegram bot for notifications.
6. Upload the provided `.ino` code to the Wemos D1 R2, making sure to update WiFi credentials and API tokens.

## Usage

1. Connect the monitoring device to the power source you want to monitor.
2. The system will automatically start collecting data and sending it to InfluxDB.
3. Access the Grafana dashboard to view real-time and historical energy consumption data.
4. Receive instant notifications on Telegram for any detected anomalies.

## Dashboard

The Grafana dashboard provides a comprehensive view of energy consumption data, including:

- Real-time voltage, current, and power graphs
- Energy consumption trends
- Power factor and frequency indicators

## Images

Here are some images from the project:

1. **Hardware Setup**
   ![Hardware Setup](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/Energy_Consumption_Monitor_Device.jpg)

2. **Data Flow Diagram**
   ![Data Flow Diagram](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/blok_diagram.jpg)

3. **Sketch Diagram**
   ![Sketch Diagram](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/Rangkaian_alat.png)

4. **Grafana Dashboard**
   ![Grafana Dashboard](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/Grafana-Dashboard.png)

5. **InfluxDB Dashboard**
   ![InfluxDB Dashboard](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/InfluxDB-Dashboard.png)

6. **Telegram Notification**
   - ![Telegram Warning Notification](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/Warning-Notif.png)
   - ![Telegram Resolved Notification](https://github.com/Ahmad-mufied/energy-consumption-monitor/blob/main/images/Resolved-Notif.png)



