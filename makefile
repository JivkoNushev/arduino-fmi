BAUDRATE = 115200
PLATFORM = arduino:avr:uno
PORT = /dev/ttyACM0

.PHONY: learn

learn-upload:
	arduino-cli compile --upload --fqbn ${PLATFORM} -p ${PORT} learn

listen: 
	picocom -b ${BAUDRATE} ${PORT}

learn: learn-upload listen
