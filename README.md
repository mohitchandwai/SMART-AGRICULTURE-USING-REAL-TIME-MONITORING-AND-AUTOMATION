# SMART-AGRICULTURE-USING-REAL-TIME-MONITORING-AND-AUTOMATION
1. 🤖 [Introduction](#introduction)
2. ⚙️ [Tech Stack](#tech-stack)
3. 🔋 [Features](#features)
4. 💻 [Youtube tutorial](#youtube)
5. 🤸 [Quick Start](#quick-start)
## <a name="introduction">🤖 Introduction</a>
We combine the power of Amazon Web Services' (AWS) Internet of Things (IoT) platform with the versatile ESP8266 microcontroller in this project. Here's how to connect an ESP8266 device to AWS IoT Core using the MQTT (Message Queuing Telemetry Transport) protocol, enabling secure bidirectional communication between the device and the cloud.
This integration provides a solid foundation for building scalable and secure IoT applications. We'll learn how to establish WiFi connection, synchronize time with NTP servers, configure SSL/TLS certificates for secure communication, and send and receive data in JSON format via MQTT.

This project leverages a cloud based machine learning model, deployed using Flask, to predict the suitability of environmental conditions for 22 unique crops.
The model analyzes key environmental factors such as temperature, humidity, soil moisture, pH, and sunlight exposure to determine if they are conducive to the healthy growth of a specific crop. The model was trained on a dataset of 2201 records compiled from [sources, e.g., agricultural databases, research institutions, publicly available weather data]. A random forest model was trained to predict crop suitability. The model achieved a preliminary accuracy of 95.6 on a held-out test set. A user-friendly interface allows users to select a specific crop and input relevant environmental data to receive a suitability prediction. This tool can assist farmers in making informed decisions about crop selection and planting schedules, leading to increased yields and more efficient resource utilization. Future work will focus on expanding the model to include more crops and incorporating real-time weather data.
Here we apply machine learning for real time monitoring.
Whole infastucture is cloud based for its scalablity and reliablity.
![undefined (2)](https://github.com/user-attachments/assets/cd550c0b-d1d8-4ddd-b49d-324a53a0c30a)
## <a name="tech-stack">⚙️ Tech Stack</a>

💎 ESP8266 

💎 Arduino Framework

💎 AWS IoT Core

💎 MQTT Protocol

💎 ArduinoJson Library

## <a name="features">🔋 Features</a>

👉 WiFi Connection: It connects to the internet using WiFi, enabling communication with other devices and services.

👉 Secure Connection to AWS IoT: It securely connects to AWS IoT, a cloud service, to exchange data without unauthorized access.

👉 Sending Random Data: It sends randomly generated temperature and humidity data in a format that's easy for other devices to understand.
## <a name="quick-start">🤸 Quick Start</a>

Follow these steps to set up the project locally on your machine.

**Prerequisites**

Make sure you have the following installed on your machine:

- [Git](https://git-scm.com/)
- [Arduino IDE](https://www.arduino.cc/en/software)

Install Board:
- [esp8266 by ESP8266 Community](https://github.com/esp8266/Arduino)

Install library:
- [PubSubClient by Nick O’Leary](https://pubsubclient.knolleary.net/)
- [ArduinoJson by Benoit Blanchon](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties)
