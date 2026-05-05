# AI-Enabled-Nutrient-Balancing-System
Smart Hydroponic AI System: Predictive Edge Computing

Welcome to the repository for the Smart Hydroponic AI System. This project provides a fully automated, predictive edge-computing solution designed for smart greenhouses and commercial indoor farming.

By integrating an IoT sensor array (ESP32) with a hybrid Deep Learning architecture (CNN-LSTM) and the Firebase Realtime Database, this system transcends traditional reactive farming. It actively predicts impending nutrient deficiencies or chemical anomalies 15 minutes into the future, automatically actuating physical dosing pumps to correct the water chemistry before the crop experiences any physiological stress.

Key Features & Application Context

Based on our deployment frameworks, this system provides several critical advantages for commercial agriculture:

Automated Preemptive Detection: The CNN-LSTM model processes 15-step time-series windows to predict chemical momentum. It flags rapid TDS or pH drifts and resolves incidents faster than periodic manual water testing.

Two-Stage Emergency Alert System: When a stress event is predicted, the system generates a local dashboard alert, providing the grower a brief window to override the action (e.g., during manual reservoir cleaning). If not canceled, it escalates to automatically trigger the dosing motors via Firebase.

Edge-to-Cloud Integration: Utilizes a dual-computer edge architecture and Firebase push protocols to minimize data transmission. It avoids continuous streaming of raw, heavy sensor data, reducing bandwidth costs and preserving corporate farm privacy.

Non-Invasive Usability: The sensors are discreetly mounted directly in the primary nutrient reservoir using 3D-printed TPU mounts. Unlike camera-based systems, it captures no visual data, protecting proprietary farm layouts and ensuring peace of mind.

Repository File Descriptions

IoTData_25K_without_interpolation_2025-05-29 10-24-09 (1).csv: The foundational historical dataset containing over 25,000 real-world telemetry records (pH, TDS, water level, temperature, humidity). Used to train the AI to understand optimal growth cycles versus deficiency events.

AI_PBL (1).ipynb: The primary Deep Learning pipeline. This Jupyter Notebook handles data loading, MinMax scaling, sequence generation (15-step windows), and the complete training loop for the hybrid Conv1D and Bidirectional LSTM model.

scaler_features (1).pkl & scaler_targets (1).pkl: Serialized scikit-learn MinMax scalers. These are absolutely crucial for deployment, ensuring that live, unpredictable greenhouse data is compressed using the exact same mathematical proportions the AI learned during its training phase.

Test_model.ipynb: The live deployment and edge-actuation script. This notebook connects securely to your Firebase database, buffers live sequential data from the ESP32, runs the AI inference, and pushes automated pump commands (e.g., nutrient_pump, ph_down_pump) back to Firebase to trigger the hardware.

How to Run the Project

Prerequisites

Ensure you have Python 3.8+ installed along with the following required libraries:

pip install pandas numpy tensorflow scikit-learn matplotlib joblib firebase-admin



Step 1: Train the AI Model

Open AI_PBL (1).ipynb in Jupyter Notebook or Google Colab.

Ensure the IoTData_25K...csv dataset is in the same directory (or uploaded to your Colab session).

Run all cells. This script will preprocess the data, train the CNN-LSTM model, and output three crucial files to your directory:

hydroponics_ai_model.keras (The trained neural network weights)

scaler_features.pkl

scaler_targets.pkl

(Note: If you already have the .pkl files uploaded in this repository, you can use them, but you must run the training script at least once to generate the .keras model file).

Step 2: Configure Firebase Setup

To run the live edge-actuation script, you must securely connect to Firebase.

Go to your Firebase Console -> Project Settings -> Service Accounts.

Click Generate new private key and download the JSON file.

Rename this file to match the expected name in the script (e.g., hydrosmart-pbl-firebase-adminsdk...json) or update the JSON_FILE variable in Test_model.ipynb to match your downloaded file.

Step 3: Run Live Inference and Automation

Open Test_model.ipynb.

Ensure the .keras model, the two .pkl scaler files, and your Firebase JSON credentials are in the working directory.

Run the notebook.

The system will automatically begin polling your Firebase sensors/ node, build a 15-step historical window, predict the farm's state 15 minutes into the future, and write the appropriate automated motor commands to the pumps/ node.

Security and Privacy

This project adheres to strict data minimization principles. Firebase connections are encrypted, and no continuous proprietary yield data or camera feeds are transmitted, thereby preventing unauthorized access and corporate espionage.
