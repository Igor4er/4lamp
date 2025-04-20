#include <Arduino.h>
#include <ESPping.h>

#ifndef WIFI_SSID
#define WIFI_SSID "fallback_ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "fallback_password"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;


const IPAddress internetIP(9, 9, 9, 9);
const IPAddress phoneIP(192, 168, 88, 44);

const int gpioPin = 0;

const unsigned long internetCheckInterval = 5 * 60 * 1000;    // 5 min
const unsigned long phonePingInterval = 5 * 1000;             // 5 sec
const unsigned long maxPhoneTimeout = 60 * 45 * 1000;         // 1 min

unsigned long lastInternetCheck = 0;
unsigned long lastPhonePing = 0;
unsigned long lastPhoneSuccess = 0;

bool internetOK = false;
bool currentState = false;
bool previousState = false;

void setupWiFi() {
  WiFi.mode(WIFI_STA);  // not AP mode
  WiFi.setSleepMode(WIFI_LIGHT_SLEEP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect. Repeating whole process");
    return setupWiFi();
  }
}

void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected to save power");
}

bool pingInternet() {
  Serial.print("Pinging Internet... ");
  if (Ping.ping(internetIP, 1)) {
    Serial.println("OK");
    return true;
  } else {
    Serial.println("FAILED");
    return false;
  }
}

bool pingPhone() {
  Serial.print("Pinging Phone... ");
  if (Ping.ping(phoneIP, 1)) {
    Serial.println("OK");
    return true;
  } else {
    Serial.println("FAILED");
    return false;
  }
}

void updateGpioState() {
  unsigned long now = millis();
  
  if (!internetOK) {
    currentState = false;
  } else {
    currentState = (now - lastPhoneSuccess <= maxPhoneTimeout) && lastPhoneSuccess != 0;
  }
  
  if (currentState != previousState) {
    Serial.print("GPIO0 changed to ");
    Serial.println(currentState ? "HIGH" : "LOW");
    digitalWrite(gpioPin, currentState ? HIGH : LOW);
    previousState = currentState;
  }
}

void setup() {
  pinMode(gpioPin, OUTPUT);
  digitalWrite(gpioPin, LOW);
  
  Serial.begin(115200);
  delay(100);
  Serial.println("\nStarting up...");
  
  setupWiFi();
  
  if (WiFi.status() == WL_CONNECTED) {
    internetOK = pingInternet();
    lastInternetCheck = millis();
    
    if (internetOK) {
      if (pingPhone()) {
        lastPhoneSuccess = millis();
      }
      lastPhonePing = millis();
    }
    
    updateGpioState();
  }
}

void loop() {
  unsigned long now = millis();
  bool needWiFi = false;
  
  // Determine if we need to connect to WiFi for checks
  if ((now - lastInternetCheck >= internetCheckInterval) || 
      (internetOK && (now - lastPhonePing >= phonePingInterval))) {
    needWiFi = true;
  }
  
  if (needWiFi && WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }
  
  if (now - lastInternetCheck >= internetCheckInterval && WiFi.status() == WL_CONNECTED) {
    internetOK = pingInternet();
    lastInternetCheck = now;
  }
  
  if (internetOK && (now - lastPhonePing >= phonePingInterval) && WiFi.status() == WL_CONNECTED) {
    if (pingPhone()) {
      lastPhoneSuccess = now;
    }
    lastPhonePing = now;
  }
  
  updateGpioState();
}
