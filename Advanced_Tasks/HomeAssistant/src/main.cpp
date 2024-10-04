// #include <WiFiS3.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// 设置WIFI
const char *ssid = "Mi4A-STAR";
const char *password = "star123!";
// 设置MQTT(EMQX)
const IPAddress mqttServer(192, 168, 31, 198);
const uint16_t mqtt_port = 1883;
const char *mqtt_user = "star";
const char *mqtt_pass = "sora";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectToWiFi()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // 等待WiFi连接,成功连接后输出成功信息
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTTServer()
{
  String clientId = "ArduinoClient";

  Serial.print("Attempting MQTT connection...");
  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str(), mqtt_user, mqtt_pass))
  {
    Serial.println("MQTT Server Connected.");
    Serial.print("Server Address: ");
    Serial.println(mqttServer);
    Serial.print("ClientId: ");
    Serial.println(clientId);
  }
  else
  {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
    // 解析 JSON 消息
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, payload);
  
  // 检查解析是否成功
  if (error)
  {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  serializeJsonPretty(doc, Serial);
}

void setup()
{
  Serial.begin(9600);
  // 连接WiFi
  connectToWiFi();

  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer, mqtt_port);
  mqttClient.setCallback(callback);
  // 连接MQTT服务器
  connectToMQTTServer();

  mqttClient.subscribe("testtopic/#");
}

void loop()
{
  if (mqttClient.connected())
  {                    // 如果开发板成功连接服务器
    mqttClient.loop(); // 保持客户端心跳
  }
  else
  {                        // 如果开发板未能成功连接服务器
    connectToMQTTServer(); // 则尝试连接服务器
  }
}
