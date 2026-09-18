<img width="1600" height="772" alt="Battery_Measurement" src="https://github.com/user-attachments/assets/02eb83c3-b98d-4172-9462-ed484cfb165b" />
<img width="1600" height="602" alt="Health_indicators" src="https://github.com/user-attachments/assets/91bf56b8-9eef-4e02-abdf-cfbeaf234592" />
High-Altitude Electrical & Electronic Equipment Protection System-PROTECT Circuit Diagram

Overview

This folder contains the circuit diagram and hardware connection details of the **HA-PROTECT (High-Altitude Electrical & Electronic Equipment Protection System).

The circuit is designed around an ESP32 microcontroller, which acts as the central control unit. It receives data from multiple sensors, processes the environmental and battery parameters, determines the system risk level, and controls the required protection mechanisms.

System Components

The circuit consists of the following major components:

* ESP32 - Main microcontroller and processing unit
* Temperature and Humidity Sensor - Monitors environmental temperature and humidity
* Pressure Sensor - Measures atmospheric pressure associated with high-altitude conditions
* Battery Monitoring Circuit - Monitors battery voltage and condition
* OLED Display - Displays real-time sensor readings and system status
* Heater - Provides heating when low-temperature conditions are detected
* Fan - Provides cooling when excessive temperature is detected
* Power Supply - Provides the required electrical power to the system

Circuit Architecture

The ESP32 is connected to the environmental and battery monitoring sensors. The sensors continuously provide measurement data to the ESP32.

The ESP32 processes these readings and evaluates the operating conditions of the system. Based on the detected conditions and calculated risk level, the controller can activate the heater or fan to maintain suitable operating conditions.

The OLED display provides real-time information such as:

* Temperature
* Humidity
* Atmospheric pressure
* Battery voltage
* Risk level
* Protection status

Working Principle

The circuit operates through the following sequence:

1. Sensors collect environmental and battery parameters.
2. The sensor values are sent to the ESP32.
3. The ESP32 processes and analyzes the collected data.
4. The system identifies the corresponding risk condition.
5. The risk level is displayed on the OLED.
6. The appropriate protection mechanism is activated.
7. The system continuously monitors the parameters and updates the protection response.

Protection Logic

The system is designed to respond to abnormal operating conditions.

For example:

* Low temperature: Heater can be activated.
* High temperature: Fan can be activated.
* Abnormal atmospheric pressure: The system can identify a high-altitude environmental condition.
* Low battery voltage: The system can indicate a battery-related risk.
* Multiple abnormal parameters: The system can increase the overall risk level.

Circuit Diagram

The diagram illustrates the electrical connections between the ESP32, sensors, display, power supply, and protection devices.

Simulation

The circuit has been developed and tested using an online simulation platform.

Simulation: Open Simulation

The simulation is used to verify sensor readings, controller logic, display output, and protection-device operation before hardware implementation.

Sample Output

TIME: 535 TEMP: 125.0C | EQUIP: 125.0C ALT: 0m | BAT: 0% LIGHT: 54 STATUS: CRITICAL
FAULT CHECK
FAULT: EQUIPMENT OVERHEATING
FAULT: BATTERY CRITICALLY LOW
FAULT: LIGHT LEVEL TOO LOW

Purpose of the Circuit

The main purpose of this circuit is to demonstrate how an embedded monitoring and protection system can be implemented for electrical and electronic equipment operating in high-altitude environments.

The design combines real-time sensing, data processing, risk identification, display, and automatic protection into a single ESP32-based system.

Future Improvements

The circuit can be further enhanced by integrating:

Battery State-of-Health estimation
Predictive risk analysis
Offline data logging
Wi-Fi-based monitoring
Cloud dashboard
Additional environmental sensors
Advanced predictive algorithms
Real-time alerts and notifications

Project

Project Name: HA-PROTECT
Domain: Electrical and Electronic Equipment Protection
Controller: ESP32
Application: High-Altitude Environment Monitoring and Protection
