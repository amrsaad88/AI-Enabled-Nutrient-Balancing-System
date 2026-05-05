#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// Required by the Firebase library
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// --- SENSOR CONFIGURATION ---
#define DHTPIN 4
#define LDRPIN 34
#define WATERPIN 35
#define DHTTYPE DHT22

// TDS UART Pins (Hardware Serial 2)
#define RXD2 16
#define TXD2 17

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial SerialTDS(2);

// --- FIREBASE & WIFI CONFIGURATION ---
// IMPORTANT: Update these with your real credentials before uploading
#define WIFI_SSID "test"
#define WIFI_PASSWORD "12345678"
#define API_KEY "AIzaSyBgQEsRK4YFcZymGNb_iwRx5te5ykW8zXM"
#define DATABASE_URL "https://hydrosmart-pbl-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define USER_EMAIL "test2@test.com"
#define USER_PASSWORD "12345678"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Timers for non-blocking delays
unsigned long lastSerialMillis = 0;
unsigned long lastFirebaseMillis = 0;

float currentTDS = 0.0;
float waterTemp = 0.0;

// Command to request data from the UGE TDS sensor
byte requestCommand[] = {0xA0, 0x00, 0x00, 0x00, 0x00, 0xA0};

void setup() {
  Serial.begin(115200);
  
  // Initialize UGE TDS Serial (9600 baud)
  SerialTDS.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  dht.begin();
  analogReadResolution(12);

  // 1. Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to Wi-Fi!");

  // 2. Initialize Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; 
  
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(2); 
  config.timeout.serverResponse = 10 * 1000;

  Serial.println("--- System Online: [A-Temp | Hum | Light | W-Lvl | TDS | W-Temp] ---");
}

void loop() {
  // --- LOCAL READING & SERIAL PRINT (Every 2 Seconds) ---
  if (millis() - lastSerialMillis > 2000) {
    lastSerialMillis = millis();

    // 1. TDS Request & Parse
    SerialTDS.write(requestCommand, sizeof(requestCommand));
    delay(150); // Buffer wait
    if (SerialTDS.available() >= 6) {
      byte response[6];
      SerialTDS.readBytes(response, 6);
      currentTDS = (response[1] << 8) | response[2]; 
      waterTemp = ((response[3] << 8) | response[4]) / 10.0; 
    }

    // 2. Read Other Sensors
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int light = analogRead(LDRPIN);
    int waterLvl = analogRead(WATERPIN);

    // 3. Single Line Print Output
    Serial.print("DATA: ");
    Serial.print(t);      Serial.print("C | ");
    Serial.print(h);      Serial.print("% | L:");
    Serial.print(light);  Serial.print(" | W-Lvl:");
    Serial.print(waterLvl); Serial.print(" | TDS:");
    Serial.print(currentTDS); Serial.print("ppm | W-T:");
    Serial.print(waterTemp); Serial.println("C");

    // --- FIREBASE CLOUD SYNC (Every 10 Seconds) ---
    if (Firebase.ready() && (millis() - lastFirebaseMillis > 10000 || lastFirebaseMillis == 0)) {
      lastFirebaseMillis = millis();

      if (!isnan(h) && !isnan(t)) {
        Firebase.RTDB.setFloat(&fbdo, F("/sensors/ambient/temp"), t);
        Firebase.RTDB.setFloat(&fbdo, F("/sensors/ambient/humidity"), h);
      }
      
      Firebase.RTDB.setInt(&fbdo, F("/sensors/light"), light);
      Firebase.RTDB.setInt(&fbdo, F("/sensors/water_level"), waterLvl);
      Firebase.RTDB.setFloat(&fbdo, F("/sensors/water/tds"), currentTDS);
      Firebase.RTDB.setFloat(&fbdo, F("/sensors/water/temp"), waterTemp);

      Serial.println(">>> CLOUD SYNC: Data successfully pushed to Firebase! <<<");
    }
  }
}