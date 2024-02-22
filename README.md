# GARDEN SENSOR PROJECT

This project creates a custom garden sensor for anyone who likes gardening, data, and nonsense.

The arduino will join wifi, read from OneWire sensors to get temperature at 9 different locations, and send that temperature data out via udp to a Gravwell logging server (https://www.gravwell.io) for analysis. Temperature is read once every 3 seconds. NTP is updated once every 10.

## WiFi Creds

The wifisekrits.h file is set to ignore via .gitignore. This file should contain your creds for wifi access.


## Arduino MKR Wifi 1010
https://docs.arduino.cc/hardware/mkr-wifi-1010



## TEMP SENSORS

https://www.makerguides.com/ds18b20-arduino-tutorial/


### wiring

https://www.makerguides.com/wp-content/uploads/2020/08/multiple-DS18B20-digital-temperature-sensors-with-Arduino-connections-wiring-diagram-schematic-circuit-tutorial-1-768x388.png

Topologies:
https://www.analog.com/en/technical-articles/guidelines-for-reliable-long-line-1wire-networks.html

amz link:
https://www.amazon.com/gp/product/B012C597T0/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

#### ETHERNET JOINS

custom to this project. I have iused ethernet wiring to extend the OneWire for each sensor using:
Orange = 5v --> Red on temp sensor
WhiteOrange = Ground --> black on sensor
Green = Data/Onewire --> yellow on sensor


### DATA ENRICHMENT TABLE

Addresses and temperature sensor calibration for this project

```
index,address,lowdiff,highdiff,avgdiff,bed,desc
S6,2831B658D4E13CF8,1.84,-0.53,0.655,2,todo
S1,287D3C58D4E13C86,-1.08,-1.01,-1.045,4,todo
S9,28880458D4E13CA5,0.09,-2.15,-1.03,7,todo
S4,28A28F58D4E13C67,0.25,-2.72,-1.235,1,todo
S10,28B25058D4E13C0D,0.06,-0.66,-0.3,6,todo
S8,28B33D58D4E13CC7,0.68,-0.93,-0.125,9,todo
S7,28B5AC58D4E13C0F,-0.51,-1.47,-0.99,3,todo
S3,28D36858D4E13C43,2.19,0.17,1.18,5,todo
S2,28EFD158D4E13C6F,-0.09,-0.28,-0.185,8,todo

```


## MOISTURE SENSORS

https://www.amazon.com/gp/product/B07SYBSHGX/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1


Analog signals are going to be rough. I don't think we can transmit this over meaningful distances, since we kind of need to do a star topology from the arduino out to each of the 9+ beds. Digital moisture sensors are probably needed instead.

### Moisture sensor baselines

Initial attempt:
Taken from sensor in a breadboard near arduino
dry in the air: ~870
in a cup of water: ~400

Subsequent attempts demonstrated wild readings, thus I scrapped it for now
