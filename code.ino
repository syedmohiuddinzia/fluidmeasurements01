#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>  // Include the ArduinoJson library

#define SENSOR_PIN_1 2
#define SENSOR_PIN_2 3
#define SENSOR_PIN_3 4
#define SENSOR_PIN_4 5

const int flowSensor1 = A7;
const int flowSensor2 = A6;
const int pressureSensorPin1 = A5;  // Pressure sensor 1 pin
const int pressureSensorPin2 = A4;  // Pressure sensor 2 pin

OneWire oneWire1(SENSOR_PIN_1);
OneWire oneWire2(SENSOR_PIN_2);
OneWire oneWire3(SENSOR_PIN_3);
OneWire oneWire4(SENSOR_PIN_4);

DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);
DallasTemperature sensor3(&oneWire3);
DallasTemperature sensor4(&oneWire4);

int X1, Y1, X2, Y2;
float TIME1 = 0, TIME2 = 0;
float FREQUENCY1 = 0, FREQUENCY2 = 0;
float WATER1 = 0, WATER2 = 0;
float TOTAL1 = 0, TOTAL2 = 0;
float LS1 = 0, LS2 = 0;
float pressure1, pressure2;
float voltage1, voltage2;

void readPressures(float& pressure1, float& pressure2) {
  int analogValue1 = analogRead(pressureSensorPin1);
  int analogValue2 = analogRead(pressureSensorPin2);

  // Convert analog values to voltage (assuming 5V supply)
  voltage1 = analogValue1 * (5.0 / 1023.0);
  voltage2 = analogValue2 * (5.0 / 1023.0);

  // Convert voltage to pressure (assuming a linear relationship)
  // Pressure range: 0 ~ 1.2 MPa
  // Output voltage range: 0.5 ~ 4.5 V
  pressure1 = (voltage1 - 0.5) * (1.2 / (4.5 - 0.5)) * 1000;
  pressure2 = (voltage2 - 0.5) * (1.2 / (4.5 - 0.5)) * 1000;
}

String generateJSON() {
  StaticJsonDocument<512> doc;  // Create a JSON document with a capacity of 512 bytes

  // Read sensor values
  sensor1.requestTemperatures();
  sensor2.requestTemperatures();
  sensor3.requestTemperatures();
  sensor4.requestTemperatures();
  readPressures(pressure1, pressure2);

  // Add sensor data to the JSON document
  doc["flowSensor1"] = WATER1;
  doc["flowSensor2"] = WATER2;
  doc["temperature1"] = sensor1.getTempCByIndex(0);
  doc["temperature2"] = sensor2.getTempCByIndex(0);
  doc["temperature3"] = sensor3.getTempCByIndex(0);
  doc["temperature4"] = sensor4.getTempCByIndex(0);
  doc["pressure1"] = pressure1;
  doc["pressure2"] = pressure2;

  // Serialize the JSON document to a string
  String jsonString;
  serializeJson(doc, jsonString);

  return jsonString;
}

void setup() {
  sensor1.begin();
  sensor2.begin();
  sensor3.begin();
  sensor4.begin();
  Serial.begin(9600);
}

void loop() {
  // Read flow sensor 1
  X1 = pulseIn(flowSensor1, HIGH);
  Y1 = pulseIn(flowSensor1, LOW);
  TIME1 = X1 + Y1;
  FREQUENCY1 = 1000000 / TIME1;
  WATER1 = FREQUENCY1 / 7.5;
  LS1 = WATER1 / 60;
  
  // Read flow sensor 2
  X2 = pulseIn(flowSensor2, HIGH);
  Y2 = pulseIn(flowSensor2, LOW);
  TIME2 = X2 + Y2;
  FREQUENCY2 = 1000000 / TIME2;
  WATER2 = FREQUENCY2 / 7.5;
  LS2 = WATER2 / 60;
  
  // Print flow sensor 1 readings
  if (FREQUENCY1 >= 0) {
    if (isinf(FREQUENCY1)) {
//      Serial.println("Sensor 1 - VOL.: 0.00 L/M");
    } else {
      TOTAL1 += LS1;
//      Serial.print("Sensor 1 - VOL.: ");
//      Serial.print(WATER1);
//      Serial.println(" L/M");
//      Serial.print("Sensor 1 - TOTAL: ");
//      Serial.print(TOTAL1);
//      Serial.println(" L");
    }
  }

  // Print flow sensor 2 readings
  if (FREQUENCY2 >= 0) {
    if (isinf(FREQUENCY2)) {
//      Serial.println("Sensor 2 - VOL.: 0.00 L/M");
    } else {
      TOTAL2 += LS2;
//      Serial.print("Sensor 2 - VOL.: ");
//      Serial.print(WATER2);
//      Serial.println(" L/M");
//      Serial.print("Sensor 2 - TOTAL: ");
//      Serial.print(TOTAL2);
//      Serial.println(" L");
    }
  }
    readPressures(pressure1, pressure2);
  // Print sensor data in JSON format
  //Serial.println(generateJSON());
  Serial.println("voltage1: " + String(voltage1) + " V");
  Serial.println("pressure1: " + String(pressure1) + " K Pa");
  Serial.println("voltage2: " + String(voltage2) + " V");
  Serial.println("pressure2: " + String(pressure2) + " K Pa");
  Serial.println("x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x");
  delay(1000);
}
