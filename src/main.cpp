#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClientSecure net;
PubSubClient client(net);

void saveCertificateToFS(DynamicJsonDocument doc)
{
  DynamicJsonDocument pem(4000);
  pem["certificatePem"] = doc["certificatePem"];
  pem["privateKey"] = doc["privateKey"];
  File file = SPIFFS.open("/aws.json", "w");
  if (!file)
  {
    Serial.println("failed to open config file for writing");
  }
  serializeJson(pem, Serial);
  serializeJson(pem, file);
  file.close();
}

void registerThing(DynamicJsonDocument doc)
{
  const char *certificateOwnershipToken = doc["certificateOwnershipToken"];
  DynamicJsonDocument reqBody(4000);
  reqBody["certificateOwnershipToken"] = certificateOwnershipToken;
  reqBody["parameters"]["SerialNumber"] = WiFi.macAddress();
  char jsonBuffer[4000];
  serializeJson(reqBody, jsonBuffer);
  Serial.println("Sending certificate..");
  client.publish("$aws/provisioning-templates/esp32_fleet_prov_template/provision/json", jsonBuffer);
}

void messageHandler(String topic, byte *payload, int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
  DynamicJsonDocument doc(length);
  deserializeJson(doc, payload);
  if (topic == "$aws/certificates/create/json/accepted")
  {
    saveCertificateToFS(doc);
    registerThing(doc);
  }
  else if (topic == "$aws/provisioning-templates/esp32_fleet_prov_template/provision/json/accepted")
  {
    Serial.println("Register things successfully.");
    Serial.println("Restart in 5s.");
    sleep(5);
    ESP.restart();
  }
}

void connectToAWS(DynamicJsonDocument cert)
{
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(cert["certificatePem"]);
  net.setPrivateKey(cert["privateKey"]);
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  // Create a message handler
  client.setCallback(messageHandler);
  client.setBufferSize(4000);
  Serial.print("Connecting to AWS IOT.");
  client.connect(THING_GROUP);
  if (!client.connected())
  {
    Serial.println("Timeout!");
    ESP.restart();
  }
  Serial.println("Connected");
  String subscriptionTopic = String(AWS_IOT_SUB_TOPIC) + "_" + WiFi.macAddress();
  char topic[50];
  subscriptionTopic.toCharArray(topic, 50);
  Serial.printf("Subscription topic: %s", topic);
  Serial.println();
  client.subscribe(topic);
}

void createCertificate()
{
  Serial.println("No file content.");
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  // Create a message handler
  client.setCallback(messageHandler);
  // Set buffer size for receive a certificate.
  client.setBufferSize(4000);
  Serial.print("Connecting to AWS IOT.");
  client.connect(THING_GROUP);
  if (!client.connected())
  {
    Serial.println("Timeout!");
    ESP.restart();
  }
  Serial.println("Connected");
  client.subscribe("$aws/certificates/create/json/accepted");
  client.subscribe("$aws/certificates/create/json/rejected");
  client.subscribe("$aws/provisioning-templates/esp32_fleet_prov_template/provision/json/accepted");
  client.subscribe("$aws/provisioning-templates/esp32_fleet_prov_template/provision/json/rejected");
  Serial.println("Create certificate..");
  client.publish("$aws/certificates/create/json", "");
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected.");
  // Init SPIFFS.
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Read AWS config file.
  File file = SPIFFS.open("/aws.json", "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  DynamicJsonDocument cert(4000);
  auto deserializeError = deserializeJson(cert, file);
  if (!deserializeError)
  {
    if (cert["certificatePem"])
    {
      connectToAWS(cert);
    }
  }
  else
  {
    createCertificate();
  }
  file.close();
}

void loop()
{
  client.loop();
}