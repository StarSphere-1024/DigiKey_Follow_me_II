#include <Arduino.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_SHT4x.h>
#include <string> // 添加string库

// 设置WIFI和MQTT参数

const char *ssid = "Mi4A-STAR";
const char *password = "star123!";
const IPAddress mqttServer(192, 168, 31, 198);
const uint16_t mqttPort = 1883;
const char *mqttUser = "star";
const char *mqttPass = "sora";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Adafruit_SHT4x sht40 = Adafruit_SHT4x();

// 连接WiFi
void connectToWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// 连接MQTT服务器
void connectToMQTTServer()
{
  String clientId = "ArduinoClient";
  Serial.print("Attempting MQTT connection...");
  if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPass))
  {
    Serial.println("MQTT Server Connected.");
  }
  else
  {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
  }
}

// 发布传感器配置信息
void publishSensorConfig(const char *sensor, const char *name, const char *unit, const char *unique_id)
{
  String topic = String("homeassistant/sensor/") + sensor + "/config";      // 自动补全topic
  String state_topic = String("homeassistant/sensor/") + sensor + "/state"; // 自动补全topic
  StaticJsonDocument<256> doc;
  char buffer[256];
  doc["name"] = name;
  doc["state_topic"] = state_topic.c_str();
  doc["unit_of_measurement"] = unit;
  doc["value_template"] = "{{ value_json.state }}";
  doc["unique_id"] = unique_id;
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer);

  Serial.print(sensor);
  Serial.println("sensor config success");

  Serial.println(topic);
  Serial.println(buffer);
  Serial.println();
}

// 发布传感器状态信息
void publishSensorState(const char *sensor, float value)
{
  String topic = String("homeassistant/sensor/") + sensor + "/state"; // 自动补全topic
  StaticJsonDocument<64> doc;
  char buffer[64];
  doc["state"] = value;
  serializeJson(doc, buffer);
  mqttClient.publish(topic.c_str(), buffer);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  //  此处代码保留，可以用于接收HomeAssistant的控制信息
}

void setup()
{
  Serial.begin(9600);
  connectToWiFi();
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
  connectToMQTTServer();
  if (!sht40.begin(&Wire1))
  {
    Serial.println(F("SHT40 sensor not found!"));
    while (1)
      ;
  }
  else
  {
    Serial.print(F("SHT40 detected!\t"));
    Serial.print(F("Serial number:\t"));
    Serial.println(sht40.readSerial(), HEX);
  }
  sht40.setPrecision(SHT4X_HIGH_PRECISION);
  sht40.setHeater(SHT4X_NO_HEATER);

  publishSensorConfig("temperature", "Temperature Sensor", "°C",  "temperature_sensor");
  publishSensorConfig("humidity", "Humidity Sensor", "%", "humidity_sensor");
}

void loop()
{
  if (!mqttClient.connected())
  {
    connectToMQTTServer();
  }
  else
  {
    mqttClient.loop();
    sensors_event_t humidity, temperature;
    sht40.getEvent(&humidity, &temperature);

    float temp = temperature.temperature;
    float humi = humidity.relative_humidity;
    Serial.print("temperature:");
    Serial.println(temp);
    Serial.print("humidity:");
    Serial.println(humi);
    Serial.println();

    publishSensorState("temperature", temp);
    publishSensorState("humidity", humi);

    delay(5000);
  }
}
