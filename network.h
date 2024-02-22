//This file establishes the network connections and sends UDP packets to a given host
// In this case, the host is a Gravwell Simple Relay receiver which logs those events
// for search and analysis. https://docs.gravwell.io/



//BEGIN NETWORK DEFINITIONS
// Setup wifi
#include "wifisekrits.h"
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;  // the Wifi radio's status
long rssi;

IPAddress timeServer(132, 163, 97, 1);  // time.nist.gov NTP server

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
NTP ntp(Udp);

//Gravwell receiver info
IPAddress gravhost;
const char* gravhostipstr = "10.0.1.1";
const int gravhostport = 9458;

//END NETWORK DEFINITIONS

void printWifiStatus() {
  // print the SSID of the network
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print board IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the signal strength
  rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void initWifi(void (*errFunc)()) {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    errFunc();

    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    ledShowError();
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
}


void logMessage(char msg[]) {
  char buffer[1024];
  Udp.beginPacket(gravhost, gravhostport);
  sprintf(buffer, "time=\"%s\",host=%s,wifi_strength=%d,msg=\"%s\"\n", ntp.formattedTime("%c"), SENSOR_NAME, rssi, msg);
  Serial.print(buffer);
  Udp.write(buffer);
  Udp.endPacket();
}

void sendUpdate() {
  rssi = WiFi.RSSI();
  char buffer[1024];

  // need twice the address buffer size to convert addresses to hex
  char addrstr[17];

  for (int i = 0; i < deviceCount; i++) {
    // send UDP packet with sensor info
    Udp.beginPacket(gravhost, gravhostport);

    tempAddrToString(tempAddresses[i], addrstr);

    sprintf(buffer, "time=\"%s\",host=%s,wifi_strength=%d,sensor=%s,value=%.2f\n", ntp.formattedTime("%c"), SENSOR_NAME, rssi, addrstr, tempF[i]);
    Serial.print(buffer);
    Udp.write(buffer);
    Udp.endPacket();
  }
}
