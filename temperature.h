// Data wire is plugged into port X on the Arduino
#define ONE_WIRE_BUS 10
//static value for maximum number of sensors
#define TMPSENSORCOUNT 12

typedef void (*logger_t)(char[]);

float tempF[TMPSENSORCOUNT];
DeviceAddress tempAddresses[TMPSENSORCOUNT];
int deviceCount = 0;          // variable to store the number of devices connected dynamically
DeviceAddress deviceAddress;  // tmp variable to store a device address

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


void tempAddrToString(DeviceAddress addr, char output[]) {
  int size = 8;

  /* pointer to the first item (0 index) of the output array */
  char* ptr = &output[0];

  int i;

  for (i = 0; i < size; i++) {
    /* "sprintf" converts each byte in the "buf" array into a 2 hex string
     * characters appended with a null byte, for example 10 => "0A\0".
     *
     * This string would then be added to the output array starting from the
     * position pointed at by "ptr". For example if "ptr" is pointing at the 0
     * index then "0A\0" would be written as output[0] = '0', output[1] = 'A' and
     * output[2] = '\0'.
     *
     * "sprintf" returns the number of chars in its output excluding the null
     * byte, in our case this would be 2. So we move the "ptr" location two
     * steps ahead so that the next hex string would be written at the new
     * location, overriding the null byte from the previous hex string.
     *
     * We don't need to add a terminating null byte because it's been already 
     * added for us from the last hex string. */
    ptr += sprintf(ptr, "%02X", addr[i]);
  }
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) {
      Serial.print(", ");
    }
  }
  Serial.println();
}

uint8_t findDevices(int pin) {
  uint8_t address[8];  // buffer for holding a single address
  uint8_t count = 0;

  if (oneWire.search(address)) {  //finds a single device
    Serial.print("\nuint8_t pin");
    Serial.print(pin, DEC);
    Serial.println("[][8] = {");
    do {
      count++;
      Serial.print("\t{ ");
      Serial.print("0x");
      for (uint8_t i = 0; i < 8; i++) {
        if (address[i] < 0x10) Serial.print("0");
        Serial.print(address[i], HEX);
      }
      Serial.print(" },");
      // CHECK CRC
      if (oneWire.crc8(address, 7) == address[7]) {
        Serial.println("\t// CRC OK");
      } else {
        Serial.println("\t// CRC FAILED");
      }
      delay(100);
    } while (oneWire.search(address));

    Serial.println("};");
    Serial.print("// nr devices found: ");
    Serial.println(count);
  }

  return count;
}

void initTempSensors(logger_t logMessage) {
  deviceCount = TMPSENSORCOUNT;
  bool crcOK;
  char tmpbuf[100];
  char tmpaddr[17];
  //init temp sensor
  sensors.begin();

  // Locate the devices on the bus:
  logMessage("Getting device count");
  deviceCount = sensors.getDeviceCount();

  sprintf(tmpbuf, "Found %d devices", deviceCount);
  logMessage(tmpbuf);

  // report parasite power requirements
  if (sensors.isParasitePowerMode()) logMessage("Parasite power is ON");
  else logMessage("Parasite power is OFF");


  logMessage("Getting device details");
  // findDevices(ONE_WIRE_BUS);

  // Serial.println("Printing addresses...");
  for (int i = 0; i < deviceCount; i++) {
    sensors.getAddress(deviceAddress, i);
    tempAddrToString(deviceAddress, tmpaddr);
    // printAddress(deviceAddress);
    if (oneWire.crc8(deviceAddress, 7) == deviceAddress[7]) {
      crcOK = true;
    } else {
      crcOK = false;
    }
    sprintf(tmpbuf, "Sensor found: address=%s,index=%d,crcpass=%s", tmpaddr, i, crcOK ? "true" : "false");
    logMessage(tmpbuf);
  }
}

float getTempByAddress(DeviceAddress address) {
  // Fetch the temperature in degrees Fahrenheit for device address:
  return sensors.getTempF(address);
}

void updateTemps() {
  // call sensors.requestTemperatures() to issue a global temperature request to all devices on the bus
  sensors.requestTemperatures();  // Send the command to get temperatures

  // Display temperature from each sensor
  for (int i = 0; i < deviceCount; i++) {
    // Serial.print("Sensor ");
    // Serial.print(i + 1);
    // Serial.print(" : ");

    sensors.getAddress(tempAddresses[i], i);
    tempF[i] = sensors.getTempFByIndex(i);

    // Serial.print(tempF[i]);
    // Serial.print(" \xC2\xB0");  // shows degree symbol
    // Serial.println("F");
    delay(20);
  }
}
