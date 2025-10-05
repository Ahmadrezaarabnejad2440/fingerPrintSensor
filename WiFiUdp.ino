#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Pirooz";
const char* password = "123456789";
const unsigned int localPort = 8888;

WiFiUDP udp;
bool wifiConnected = false;

void setupWiFi() {
  WiFi.softAP(ssid, password);  // تنظیم نقطه دسترسی

  Serial.println();
  Serial.print("AP mode activated, IP address: ");
  Serial.println(WiFi.softAPIP());

  udp.begin(localPort);
  wifiConnected = true;
}

void handleUDPPackets() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(udp.remotePort());

    char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
    int len = udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents: ");
    Serial.println(packetBuffer);

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write("ok");
    udp.endPacket();
  }
}

void sendUDPData(const char* ipAddress, const unsigned int port, const char* data) {
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(data);
  udp.endPacket();
}

void setup() {
  Serial.begin(9600);
  delay(10);

  setupWiFi();
}

void loop() {
  if (!wifiConnected) {
    setupWiFi();
  }

  handleUDPPackets();

  // در اینجا می‌توانید داده مورد نظر خود را برای دستگاه مبدا ارسال کنید
  // sendUDPData("192.168.4.2", 8888, "Hello from Arduino!");

  while(Serial.available()) {
    if (Serial.readString()) {
      sendUDPData("192.168.4.2", 8888, "HELLO");
    }
  }
  
}
