#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <NTP.h>

#include "led.h"
#include "moisture.h"
#include "temperature.h"
#include "network.h"



void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // while (!Serial) {
  //   ;  // wait for serial port to connect. Needed for native USB port only
  // }

  if (gravhost.fromString(gravhostipstr)) {  // try to parse into the IPAddress
    Serial.println(gravhost);                // print the parsed IPAddress
  } else {
    Serial.println("UnParsable IP");
    ledShowError();
  }

  initWifi(ledShowError);


  ntp.ruleDST("CDT", Second, Sun, Mar, 2, -300);  // USA daylight savings time for central timezone
  ntp.ruleSTD("CST", First, Sun, Nov, 2, -360);   // USA daylight savings time for central timezone
  ntp.begin(timeServer);

  // this "reboot" msg has to go after ntp.begin. probably because we always send timestamp and before we init it's messed up?
  //I dunno. UDP packets don't even SEND despite printing to serial just fine...
  logMessage("System initializing");
  logMessage("NTP initialization");

  initTempSensors(&logMessage);
}

int secs = 0;
void loop() {
  secs++;

  updateTemps();
  // updateMoisture();

  if (secs % 10 == 0) {
    ntp.update();
    logMessage("updating NTP");
  }

  if (secs % 3 == 0) {
    updateLEDs(secs % 2);
  }

  sendUpdate();

  //primary loop delay
  delay(1000);
}
