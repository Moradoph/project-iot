#include "DHT.h"
#include <WiFi.h>
#include <LiquidCrystal.h>
#include <TimeLib.h>

const int lightSensorPin = A0;
const int ledPin = 13;
const int dhtPin = 4;
const int DHTTYPE = DHT22; 

DHT dht(dhtPin, DHTTYPE);

const int columns = 16;
const int rows = 2;

LiquidCrystal lcd(); // parameters ไปไงบ่ฮุ็ที

const char * ssid = "Trin";
const char * pwd = "1234567890";

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  dht.begin();
  lcd.begin(columns, rows);

  setSyncProvider(getTime); 

  WiFi.begin(ssid, pwd);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //  อ่านค่าความเข้มแสง
  int lightIntensity = analogRead(lightSensorPin);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (lightIntensity < 500) { 
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(ledPin, LOW); 
  }

  // ทำงานตอนความชื้นต่ำ
  if (shouldPerformTask(humidity)) { 
    lcd.setCursor(0, 0);
    lcd.print("(U w U)");
    // รดน้ำต่างๆ, ส่งข้อมูลไปเก็บ/ show blynk

  } else {
    lcd.clear();
  }

  delay(1000); 
}

bool shouldPerformTask(float humidity) {
  if (humidity < 40 || isScheduledTime()) {
    return true;
  }
  return false;
}

bool isScheduledTime() {
  if (hour() == scheduledHour && minute() == scheduledMinute) {
    return true;
  }
  return false;
}