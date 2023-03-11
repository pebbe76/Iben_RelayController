#include <WiFi.h>
#include "arduino_secrets.h"
#include "PubSubClient.h"

char const* switchTopic2 = "/RelayController/Channel1/";
char const* switchTopic3 = "/RelayController/Channel2/";
char const* switchTopic4 = "/RelayController/Channel3/";
char const* switchTopic5 = "/RelayController/Channel4/";
char const* switchTopic6 = "/RelayController/Channel5/";
char const* switchTopic7 = "/RelayController/Channel6/";
char const* switchTopic8 = "/RelayController/Channel7/";
char const* switchTopic9 = "/RelayController/Channel8/";

char ip[16];
int signalStrength = 0;
int radio_Status = WL_IDLE_STATUS;  // WiFi radio's status
int reportingFrequency = 0;
WiFiClient wifiClient;
PubSubClient mqttClient;
IPAddress ipaddress;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //while(!Serial); //neverending loop while powered by external powersource without serial communication 

  pinMode(2, OUTPUT);    
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);    
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  WiFi.setHostname("RelayController");

  // attempt to connect to WiFi network:
  Serial.println("Attempting wifi connect...");
  while (radio_Status != WL_CONNECTED) {
    reconnectWiFi();
  }

  // setup mqtt client
  mqttClient.setClient(wifiClient);
  mqttClient.setServer("192.168.1.2", 1883);
  mqttClient.setCallback(callback);
  delay(1500);
}

void loop() {
  
  signalStrength = WiFi.RSSI();
  ipaddress = WiFi.localIP();
  sprintf(ip, "%d.%d.%d.%d", ipaddress[0], ipaddress[1], ipaddress[2], ipaddress[3]);

  if (!mqttClient.connected() && WiFi.status() == 3) {
    readMQTT();
    }

  if(reportingFrequency > 500){
    //Report service data, but not constantly to offload broker
    reportMQTT();
    reportingFrequency = 0;
  }

  mqttClient.loop();
  delay(100); 
  reportingFrequency++;
}

void reconnectWiFi(){
  while (radio_Status != WL_CONNECTED) {
  //while (radio_Status != 3) {
    // Connect to WPA/WPA2 network:
    radio_Status = WiFi.begin(WIFI_SSID, WIFI_PWD);
    Serial.println(radio_Status);

    // wait 5 seconds for connection:
    delay(5000);
  }
}

void readMQTT() {
  while (radio_Status != WL_CONNECTED) {
    reconnectWiFi();
  }

  if(radio_Status == WL_CONNECTED){
    while (!mqttClient.connected()) {
      String clientName = "RelayController";
      /*
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      */
      if (mqttClient.connect((char*) clientName.c_str(),MQTT_USR, MQTT_PWD)) {   
        Serial.println("mqtt client connected");
        mqttClient.subscribe(switchTopic2);
        mqttClient.subscribe(switchTopic3);
        mqttClient.subscribe(switchTopic4);
        mqttClient.subscribe(switchTopic5);
        mqttClient.subscribe(switchTopic6);
        mqttClient.subscribe(switchTopic7);
        mqttClient.subscribe(switchTopic8);
        mqttClient.subscribe(switchTopic9);
      }
    }
  }
}

void reportMQTT(){
  if (mqttClient.connected() && WiFi.status() == 3) {
    char strength[10] = {' '} ;
    itoa (signalStrength, strength, 10); 
    mqttClient.publish("RelayController/signalstrength", strength);
    mqttClient.publish("RelayController/ipaddress", ip);
    
    //Debug
    Serial.println("MQTT reporting complete.");
  }
  else {
    Serial.println("mqtt could not be sent. No connection available.");
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (strcmp(topic, switchTopic2) == 0){
    //switchpanel(2, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(2, messageTemp);
  }
  else if (strcmp(topic, switchTopic3) == 0){
    //switchpanel(3, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(3, messageTemp);
  }
  else if (strcmp(topic, switchTopic4) == 0){
    //switchpanel(4, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(4, messageTemp);
  }
  else if (strcmp(topic, switchTopic5) == 0){
    //switchpanel(5, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(5, messageTemp);
  }
  else if (strcmp(topic, switchTopic6) == 0){
    //switchpanel(6, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(6, messageTemp);
  }
  else if (strcmp(topic, switchTopic7) == 0){
    //switchpanel(7, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(7, messageTemp);
  }
  else if (strcmp(topic, switchTopic8) == 0){
    //switchpanel(8, bool(messageTemp));
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
    switchpanel(8, messageTemp);
  }
  else if (strcmp(topic, switchTopic9) == 0){
    //switchpanel(9, bool(messageTemp));
    switchpanel(9, messageTemp);
    Serial.print("Calling switchpanel with value ");
    Serial.println(messageTemp);
  }
  else{
    Serial.print("Switchpanel never called. topic: ");
    Serial.print(topic);
    Serial.print(" message: ");
    Serial.println(messageTemp);
  }
  messageTemp = "";
}

void switchpanel(int channel, String state){
  //Debugging
  Serial.print("Switchpanel invoked on channel ");
  Serial.print(channel);
  Serial.print(" with value ");
  Serial.println(state);

  //Functionality
  String x = "";
  String t = "true";
  x = state.c_str();

  if (x == t){
    digitalWrite(channel, HIGH);
  }
  else {
    digitalWrite(channel, LOW);
  }

  //Debugging
  Serial.print("Pin state after switching: Pin no. ");
  Serial.print(channel);
  Serial.print(" (channel ");
  Serial.print(channel - 1);
  Serial.print(") is now ");
  Serial.println(digitalRead(channel));
}

/*
String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}
*/