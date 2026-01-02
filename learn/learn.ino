#include "AccelStepper.h"
#include "elevator.h"
#include "config.h"

ElevatorButtons callers;
Elevator elevator(callers);

void setup() {
    Serial.begin(SERIAL_BAUDRATE);

    callers.begin();
    elevator.begin();
}

void loop() {
    callers.update();
    elevator.update();
}
