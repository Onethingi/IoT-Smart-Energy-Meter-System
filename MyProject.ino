#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME "Energy Meter"
#define BLYNK_AUTH_TOKEN
"wkHsNPukovZdJ8QrXh30ev1LSaUPvYhc"
 #include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

char ssid[] = "POCO F5";
char pass[] = "12345678";
#define TRIG_PIN 26
#define ECHO_PIN 25
int distanceThreshold = 10; 
bool isTampering = false;

#define ZMPT_PIN 34
#define ACS_PIN 35
#define ADC_MAX 4095.0
#define ADC_VREF 3.3
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer; 

float voltageCal = 230.0;
float currentCal = 30.0;

float energy_kWh = 0.0;
unsigned long lastTime = 0;



void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Energy");
  lcd.setCursor(0, 1);
  lcd.print("Connecting Wi-Fi");
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  lcd.clear();
  lastTime = millis(); 
  timer.setInterval(1000L, checkTheftSensor); 
  timer.setInterval(15000L, updateEnergyMeter); 
}

void loop() {
  Blynk.run(); 
  timer.run(); 
}
void checkTheftSensor() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  if (distance > 0 && distance < distanceThreshold && !isTampering) {
    
    Blynk.logEvent("theft_alert", "WARNING: Physical interaction detected at
the meter!");
    Serial.println("ALERT: Tampering Detected!");
    isTampering = true;
  }
  else if (distance >= distanceThreshold) {
    isTampering = false; // Reset when the person moves away
  }
}
void updateEnergyMeter() {
  float voltage = readVoltage();
  float current = readCurrent();
  float power = voltage * current; 
  unsigned long now = millis();
  float hours = (now - lastTime) / 3600000.0;
  energy_kWh += (power * hours) / 1000.0;
  lastTime = now;
  float unitRate = 8.0;
  float totalCost = energy_kWh * unitRate;
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage, 1);
  lcd.print(" I:");
  lcd.print(current, 2);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(power, 1);
  lcd.print(" E:");
  lcd.print(energy_kWh, 2);
  
  Serial.print("Voltage: "); Serial.print(voltage);
  Serial.print(" V | Current: "); Serial.print(current);
  Serial.print(" A | Power: "); Serial.print(power);

  Serial.print(" W | Energy: "); Serial.print(energy_kWh);
  Serial.print(" kWh | Cost: Rs "); Serial.println(totalCost);
  Blynk.virtualWrite(V0, voltage); 
  Blynk.virtualWrite(V1, current); 
  Blynk.virtualWrite(V2, power); 
  Blynk.virtualWrite(V3, energy_kWh); 
  Blynk.virtualWrite(V4, totalCost); 
}
float readVoltage() {
  int samples = 1000;
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    float adc = analogRead(ZMPT_PIN);
    float v = (adc * ADC_VREF) / ADC_MAX;
    v -= ADC_VREF / 2.0; 
    sum += v * v;
  }
  float rms = sqrt(sum / samples);
  return rms * voltageCal;
}
float readCurrent() {
  int samples = 1000;
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    float adc = analogRead(ACS_PIN);
    float v = (adc * ADC_VREF) / ADC_MAX;
    v -= ADC_VREF / 2.0; 
    sum += v * v;
  }
  float rms = sqrt(sum / samples);
  return rms * currentCal;
}
