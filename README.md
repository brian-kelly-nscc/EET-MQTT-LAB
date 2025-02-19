# PLC Communications Lab: MQTT Data Transmission

This repository contains the code for a communications lab designed for the PLC course. In this lab, Arduino Uno R3 boards equipped with an Ethernet 2 shield are used to send sensor data over a network using MQTT. Students will learn how to read digital and analog inputs, configure static IPs, and publish data to an MQTT broker on an airgapped network.

## Overview

- **Hardware**: Arduino Uno R3, Ethernet 2 shield, button (with internal pull-up configuration), external LED (recommended on digital pin 7).
- **Software**: Arduino IDE 2.3.4 or later, PubSubClient library for MQTT, Ethernet library.
- **Networking**: Static IP configuration is used. The lab network is airgapped and unrouted. PLC network addresses available: **192.168.60.170** to **192.168.60.199**.
- **MQTT**: The board publishes its sensor readings to the `sensor/analog` topic and its identity on boot to the `board/boot` topic.

## Features

- **Customizable Configuration**: Easily change the board name, IP address, subnet, and MAC address at the top of the code.
- **Sensor Data Publishing**: Press the button (digital pin 2 with internal pull-up) to trigger an analog read from A0 and publish the reading along with the board name.
- **Visual Feedback**: An external LED connected to digital pin 7 flashes when an MQTT message is received, providing immediate visual feedback.
- **MQTT Communication**: Connects to an MQTT broker (set up on a VM) and exchanges messages to facilitate network-wide communication among boards.

## Getting Started

### Prerequisites

- **Hardware Requirements**:
  - Arduino Uno R3
  - Ethernet 2 Shield
  - Button (wired with a pull-up configuration)
  - External LED (with resistor) connected to digital pin 7
- **Software Requirements**:
  - Arduino IDE (version 2.3.4 or later)
  - [PubSubClient library](https://github.com/knolleary/pubsubclient) (Install via the Library Manager)
  - Ethernet library (bundled with the Arduino IDE)
- **MQTT Broker**:
  - Install and configure an MQTT broker (e.g., Mosquitto) on a VM.
  - Update the `mqtt_server` variable in the code with your broker’s IP address.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/plc-communications-lab.git
