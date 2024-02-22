// This file is not currently used by the project as moisture sensors
//	were not giving accurate readings

//BEGIN MOISTURE SENSOR DEFINITIONS
// int moisturePin = A1;
// int moistureVal = 0;
struct msensor {
  int pin;
  int val;
};

typedef struct msensor MSensor;
#define MOISTSENSORCOUNT 2
MSensor msensors[MOISTSENSORCOUNT];

//END MOISTURE SENSOR DEFINITIONS

void initMoistSensors() {
  msensors[0].pin = A1;
  msensors[0].val = 0;
  msensors[1].pin = A2;
  msensors[1].val = 0;
}

void updateMoisture() {
  // moistureVal = analogRead(moisturePin);
  for (int i = 0; i < MOISTSENSORCOUNT; i++) {
    msensors[i].val = analogRead(msensors[i].pin);
  }
}
