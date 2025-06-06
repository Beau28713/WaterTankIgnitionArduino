---

# Home Water System Automation with Arduino & Ignition SCADA

## Overview

This project was created as a practical learning experience to deepen my understanding of the **MODBUS communication protocol** and **Ignition SCADA** software.

Without access to a PLC and unable to justify purchasing one, I turned to an available **Arduino UNO WiFi Rev2** to simulate MODBUS communication. Leveraging the **Arduino MODBUS library**, I was able to implement both **MODBUS RTU (RS485)** and **MODBUS TCP** protocols.

To visualize and control the system, I used **Ignition's Maker Edition**—a free, fully featured SCADA platform for personal use. The end goal was to build a functioning SCADA-controlled **home water system** as both a practical project and an educational tool.

---

## Features

* MODBUS TCP communication between Arduino and Ignition SCADA
* Real-time data monitoring and control via a custom Ignition HMI
* Analog voltage readings sent from Arduino to Ignition using float conversion across two registers
* Arduino as a MODBUS slave device
* Home water pump and tank logic simulation
* Custom tags and scripting inside Ignition

---

## Tools & Technologies

* **Arduino UNO WiFi Rev2**
* **Arduino IDE**
* **Arduino MODBUS Library**
* **Ignition SCADA Maker Edition**
* **MODBUS TCP/IP Protocol**

---

## Goals

* Learn how MODBUS communication works in both embedded and SCADA environments
* Understand how Ignition SCADA handles data types and tag configuration
* Apply low-cost hardware to build industrial-style control systems at home

---

## Lessons Learned

* **MODBUS Register Mapping**: Arduino starts addressing at `0`, while Ignition SCADA starts at `1`. This offset must be accounted for to avoid communication issues.
* **Floating Point Handling**: Floating-point values (like voltages) must be split into two 16-bit registers when sending to Ignition.
* **SCADA Setup**: Proper device configuration and tag setup in Ignition are critical for smooth communication.

---

## Future Improvements

* Add local relay control for pump switching
* Integrate flow sensors and water level sensors
* Expand system to include auto alerts and dashboard statistics
* Explore MODBUS RTU communication via RS485

---

## Screenshots

*(Insert screenshots of the SCADA HMI, Arduino setup, or tank system here)*

---

## License

This project is open for learning and educational use. Feel free to fork or contribute.

---

## Contact

Feel free to reach out via [LinkedIn](https://www.linkedin.com/in/beaujclark/) if you’re working on similar projects or have questions.

---
