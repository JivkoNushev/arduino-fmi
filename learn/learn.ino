//
// #include "AccelStepper.h"
//
// const int dirPin = 5;
// const int stepPin = 6;
// const int enablePin = 4;
//
// const int testButtonPin = 2;
// const int testButton2Pin = 3;
//
// char serialBuffer[1024] = {0};
// volatile bool test_button_pressed = false;
// volatile uint32_t last_pressed_ms = 0;
//
// volatile bool test_button2_pressed = false;
// volatile uint32_t last_pressed2_ms = 0;
//
// uint32_t last_printed_ms = 0;
//
// int32_t current_pos = 0;
// const int32_t first_floor_pos = 200;
// const int32_t second_floor_pos = 1000;
//
// #define motorInterfaceType 1
// AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);
//
// enum State { IDLE, FIRST_FLOOR, GOING_TO_FIRST_FLOOR, SECOND_FLOOR, GOING_TO_SECOND_FLOOR };
//
// State current_state = IDLE;
//
//
// void setup() {
//     Serial.begin(115200);
//
//     pinMode(enablePin, OUTPUT);
//     digitalWrite(enablePin, LOW);
//
//     pinMode(testButtonPin, INPUT_PULLUP);
//     pinMode(testButton2Pin, INPUT_PULLUP);
//     attachInterrupt(digitalPinToInterrupt(testButtonPin), testButtonISR, RISING);
//     attachInterrupt(digitalPinToInterrupt(testButton2Pin), testButtonISR2, RISING);
//
//     myStepper.setMaxSpeed(1000);
//     myStepper.setAcceleration(50);
//     myStepper.setSpeed(200);
//
//     //
//     // callers.begin();
//     // elevator.begin();
// }
//
// void testButtonISR() {
//     if(test_button_pressed) return;
//
//     unsigned long now = millis();
//     if(now - last_pressed_ms <= 500) return;
//
//
//     test_button_pressed = true;
//     if(current_state == IDLE) {
//         current_state = FIRST_FLOOR;
//     }
//
//     last_pressed_ms = now;
// }
//
// void testButtonISR2() {
//     if(test_button2_pressed) return;
//
//     unsigned long now = millis();
//     if(now - last_pressed2_ms <= 500) return;
//
//
//     test_button2_pressed = true;
//     if(current_state == IDLE) {
//         current_state = SECOND_FLOOR;
//     }
//
//     last_pressed2_ms = now;
// }
//
// void print_pressed() {
//     uint32_t now = millis();
//     if (now - last_printed_ms < 200) return;
//
//     Serial.print("Button pressed: ");
//     Serial.println(test_button_pressed);
//
//     Serial.print("Current state: ");
//     Serial.println(current_state);
//     last_printed_ms = now;
// }
//
// void goto_first_floor() {
//     uint32_t err = first_floor_pos - current_pos;
//     myStepper.move(err);
// }
//
// void goto_second_floor() {
//     uint32_t err = second_floor_pos - current_pos;
//     myStepper.move(err);
// }
//
// void loop() {
//
//     switch(current_state) {
//         case IDLE:
//             print_pressed();
//             break;
//
//         case FIRST_FLOOR:
//             goto_first_floor();
//             current_state = GOING_TO_FIRST_FLOOR;
//             break;
//         case GOING_TO_FIRST_FLOOR:
//             if (myStepper.distanceToGo() == 0) {
//                 digitalWrite(enablePin, HIGH);
//                 current_state = IDLE;
//                 test_button_pressed = false;
//                 current_pos = first_floor_pos;
//             }
//             digitalWrite(enablePin, LOW);
//             break;
//
//
//         case SECOND_FLOOR:
//             goto_second_floor();
//             current_state = GOING_TO_SECOND_FLOOR;
//             break;
//         case GOING_TO_SECOND_FLOOR:
//             if (myStepper.distanceToGo() == 0) {
//                 digitalWrite(enablePin, HIGH);
//                 current_state = IDLE;
//                 test_button2_pressed = false;
//                 current_pos = second_floor_pos;
//             }
//             digitalWrite(enablePin, LOW);
//             break;
//
//         default:
//             break;
//     }
//
//     myStepper.run();
// }
//

#include "AccelStepper.h"
#include "elevator.h"

ElevatorButtons callers;
Elevator elevator(callers);

void setup() {
    Serial.begin(115200);

    callers.begin();
    elevator.begin();
}

void loop() {
    callers.update();
    elevator.update();
}
