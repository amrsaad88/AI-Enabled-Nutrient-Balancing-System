# AI-Enabled Smart Nutrient Balancing System for Hydroponics

## Overview
This project presents an intelligent system for optimizing nutrient management in hydroponic environments. Traditional hydroponic systems rely on reactive control mechanisms that respond only after environmental parameters exceed predefined thresholds. In contrast, this system introduces a predictive, data-driven approach that anticipates changes before they occur.

By integrating Internet of Things (IoT) sensing, cloud infrastructure, and a hybrid deep learning model (CNN-LSTM), the system enables proactive decision-making to maintain optimal plant growth conditions and reduce the risk of crop failure.

---

## Key Features
- Real-time monitoring of environmental and water quality parameters  
- Predictive modeling of system conditions up to 15 minutes in advance  
- Automated nutrient adjustment through a closed-loop control system  
- Cloud-based data storage and communication using Firebase  
- Web-based dashboard for visualization and remote interaction  

---

## System Architecture
The system is structured into four primary layers:

**1. Edge Layer (ESP32):**  
Responsible for continuous data acquisition from sensors, including pH, TDS, temperature, and humidity. Initial signal processing and filtering are performed at this stage.

**2. Cloud Layer (Firebase):**  
Acts as a centralized data repository and communication bridge between the hardware and the AI model. Enables real-time data streaming and storage.

**3. Artificial Intelligence Layer:**  
Implements a hybrid Convolutional Neural Network and Long Short-Term Memory (CNN-LSTM) model to analyze time-series data and forecast future environmental conditions.

**4. User Interface Layer:**  
Provides a web-based dashboard for real-time monitoring, visualization of predictions, and manual system control.

---

## Technologies Used
- Hardware: ESP32 microcontroller, pH sensor, TDS sensor, temperature and humidity sensors  
- Software: Python, Arduino (C++)  
- Machine Learning: TensorFlow, Keras  
- Cloud Platform: Firebase Realtime Database  
- Communication: Wi-Fi  

---

## System Operation
The system operates through a continuous data loop:

1. Sensors capture real-time environmental and nutrient data  
2. The ESP32 transmits the data to the Firebase database  
3. The AI model processes historical and live data to predict future states  
4. The system determines whether corrective action is required  
5. Commands are sent to actuators for nutrient adjustment  
6. The dashboard reflects both real-time data and predictive insights  

---

## Results
The system achieved high predictive performance and operational reliability:

- Prediction accuracy of approximately 96%  
- Forecast horizon of 15 minutes  
- Real-time response latency below 2 seconds  

The model effectively distinguishes between normal plant behavior and critical nutrient deficiencies, enabling timely intervention.

---

## Installation and Setup

### Hardware Setup
- Connect all sensors to the ESP32 microcontroller  
- Upload the firmware using the Arduino environment  

### Software Setup
```bash
# Clone the repository
git clone https://github.com/your-repository-name.git

# Install required dependencies
pip install -r requirements.txt

# Run the AI model
python main.py
