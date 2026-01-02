BAUDRATE = 115200
PLATFORM = arduino:avr:uno
PORT = /dev/ttyACM0

.PHONY: main

main-upload:
	arduino-cli compile --upload --fqbn ${PLATFORM} -p ${PORT} main

listen: 
	picocom -b ${BAUDRATE} ${PORT}

main: main-upload listen
