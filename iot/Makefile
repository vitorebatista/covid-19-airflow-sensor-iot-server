PORT = /dev/cu.usbserial-0001
FQBN = "esp8266:esp8266:nodemcuv2"
TARGET = core-install lib-install

all: $(TARGET)

board-list:
	arduino-cli board list

core-install:
	arduino-cli core install esp8266:esp8266

core-list:
	arduino-cli core list

lib-install:
	arduino-cli lib install EspMQTTClient ArduinoJson

lib-list:
	arduino-cli lib list

compile:
	arduino-cli compile --fqbn $(FQBN)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN)

help:
	@echo
	@echo "RMVS Command Line Interface."
	@echo
	@echo "Usage:"
	@echo "  make [command]"
	@echo
	@echo "Available Commands:"
	@echo "  all             Install all the core and libraries dependencies."
	@echo "  board-list      List connected boards."
	@echo "  core-install    Download the latest version of Arduino core."
	@echo "  core-list       Shows the list of installed platforms."
	@echo "  help            Help about any command."
	@echo "  lib-install     Download the latest version of Libraries."
	@echo "  lib-list        Shows a list of all installed libraries."
	@echo "  compile         Compile the sketch project and create build folder."
	@echo "  upload          Upload the sketch to your board."
	@echo

	