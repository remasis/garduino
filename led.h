//This file is just for some LED feedback and silliness

void initLEDs() {
  WiFiDrv::pinMode(25, OUTPUT);  //define green pin
  WiFiDrv::pinMode(26, OUTPUT);  //define red pin
  WiFiDrv::pinMode(27, OUTPUT);  //define blue pin
}

void ledShowError() {
  WiFiDrv::analogWrite(25, 50);
  WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 0);
}


void updateLEDs(int type) {
  if (type) {
    WiFiDrv::analogWrite(25, 10);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, 15);
  } else {
    WiFiDrv::analogWrite(25, 0);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, 0);
  }
}
