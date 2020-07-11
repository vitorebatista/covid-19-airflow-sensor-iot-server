# RMVS IOT (WORK IN PROGRESS)

## Dependencies

- Arduino
- [Arduino-cli](https://arduino.github.io/arduino-cli/installation/)

### Install the core for your board

To install the platform core and the libraries, run the following:

```sh
$ make all
```

Now verify we have installed the core and libraries properly by running:

```sh
$ make core-list

ID              Installed Latest Name
esp8266:esp8266 2.7.1     2.7.1
```

```sh
$ make lib-list
Name              Installed Available Location
ArduinoJson       6.15.2    -         user
PubSubClient      2.8       -         user
EspMQTTClient     1.8.0     -         user
PubSubClientTools 0.6       -         user
```

## Compile and upload the sketch

To compile the sketch, you run the compile command above, but before this, don't forget to configure the `secrets.h` with your wifi and broker configurations.

```sh
$ make compile
Global variables use 30476 bytes (37%) of dynamic memory, leaving 51444 bytes for local variables. Maximum is 81920 bytes.
```

This command will create an `esp8266.es8266.generic` folder inside `iot` folder. So you will need to access the url `http://rmvs_esp8266.local/` and upload the `.bin` firmware file to update your board.

But if your board is connected in your USB port, you should be able to check whether it's been recognized by running:

```sh
$ make board-list
Port                            Type              Board Name FQBN Core
/dev/cu.usbserial-0001          Serial Port (USB) Unknown
```

If you have a different port (/dev/cu.usbserial-0001), you should copy your port name and change the variable `PORT` in the `Makefile` or you can specify the port to override the default value:

```sh
$ make upload PORT=<YOUR_BOARD_PORT>
No new serial port detected.
esptool.py v2.8
Serial port /dev/cu.usbserial-0001
Connecting....
Chip is ESP8266EX
Features: WiFi
Crystal is 26MHz
MAC: dc:4f:22:5d:b6:17
Uploading stub...
Running stub...
Stub running...
Configuring flash size...
Auto-detected Flash size: 4MB
Flash params set to 0x0340
Compressed 345680 bytes to 248217...
Wrote 345680 bytes (248217 compressed) at 0x00000000 in 22.0 seconds (effective 125.9 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```
