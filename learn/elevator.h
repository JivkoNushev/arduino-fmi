#pragma once

#include "AccelStepper.h"
#include "elevator_button.h"

#include "config.h"

class Elevator {
public:
    Elevator(ElevatorButtons& callers) :
        enable_pin(MOTOR_ENABLE_PIN),
        callers(&callers),
        driver(MOTOR_INTERFACE_TYPE, MOTOR_STEP_PIN, MOTOR_DIR_PIN)
    {
    }

    void begin() {
        pinMode(this->enable_pin, OUTPUT);

        this->driver.setMaxSpeed(MOTOR_MAX_SPEED);
        this->driver.setAcceleration(MOTOR_ACCELERATION);
        this->driver.setSpeed(MOTOR_SPEED);

        this->state = State::TURN_OFF;
    }

    void update() {
        this->update_state();
        this->driver.run();
    }

private:
    enum State {
        IDLE,

        TURN_OFF,
        TURN_ON,

        GOTO_FLOOR_1,
        MOVE_TO_FLOOR_1,
        GOING_TO_FLOOR_1,
        GOTO_FLOOR_2,
        MOVE_TO_FLOOR_2,
        GOING_TO_FLOOR_2,
        GOTO_FLOOR_3,
        MOVE_TO_FLOOR_3,
        GOING_TO_FLOOR_3,

        WAITING_ON_FLOOR
    };

    AccelStepper driver;
    ElevatorButtons* callers;

    State state = IDLE;
    State goto_state = IDLE;
    uint8_t enable_pin = 0;

    uint32_t current_pos = 0;

    uint32_t start_wait_time = 0;

    void update_state() {
        switch(this->state) {
            case IDLE:
                this->goto_next_called();
                break;

            case TURN_OFF:
                digitalWrite(this->enable_pin, HIGH);
                this->handle_goto_state();
                break;

            case TURN_ON:
                digitalWrite(this->enable_pin, LOW);
                this->handle_goto_state();
                break;

            case GOTO_FLOOR_1:
                this->state = State::TURN_ON;
                this->goto_state = State::MOVE_TO_FLOOR_1;
                break;
            case MOVE_TO_FLOOR_1:
                this->driver.move(ELEVATOR_FLOOR1_POS - this->current_pos);

                this->state = State::GOING_TO_FLOOR_1;
                break;
            case GOING_TO_FLOOR_1:
                if (this->driver.distanceToGo() == 0) {
                    this->current_pos = ELEVATOR_FLOOR1_POS;
                    this->start_wait_time = millis();

                    this->state = State::TURN_OFF;
                    this->goto_state = State::WAITING_ON_FLOOR;
                }
                break;

            case GOTO_FLOOR_2:
                this->state = State::TURN_ON;
                this->goto_state = State::MOVE_TO_FLOOR_2;
                break;
            case MOVE_TO_FLOOR_2:
                this->driver.move(ELEVATOR_FLOOR2_POS - this->current_pos);

                this->state = State::GOING_TO_FLOOR_2;
                break;
            case GOING_TO_FLOOR_2:
                if (this->driver.distanceToGo() == 0) {
                    this->current_pos = ELEVATOR_FLOOR2_POS;
                    this->start_wait_time = millis();

                    this->state = State::TURN_OFF;
                    this->goto_state = State::WAITING_ON_FLOOR;
                }
                break;

            case GOTO_FLOOR_3:
                this->state = State::TURN_ON;
                this->goto_state = State::MOVE_TO_FLOOR_3;
                break;
            case MOVE_TO_FLOOR_3:
                this->driver.move(ELEVATOR_FLOOR3_POS - this->current_pos);

                this->state = State::GOING_TO_FLOOR_3;
                break;
            case GOING_TO_FLOOR_3:
                if (this->driver.distanceToGo() == 0) {
                    this->current_pos = ELEVATOR_FLOOR3_POS;
                    this->start_wait_time = millis();

                    this->state = State::TURN_OFF;
                    this->goto_state = State::WAITING_ON_FLOOR;
                }
                break;

            case WAITING_ON_FLOOR:
                if(millis() - this->start_wait_time < ELEVATOR_WAIT_TIME_MS) break;

                this->state = State::IDLE;
                break;

            default:
                break;
        }
    }

    void goto_next_called() {
        switch(this->callers->next_floor()) {
            case 1:
                this->state = State::GOTO_FLOOR_1;
                break;
            case 2:
                this->state = State::GOTO_FLOOR_2;
                break;
            case 3:
                this->state = State::GOTO_FLOOR_3;
                break;

            default:
                break;
        }
    }

    void handle_goto_state() {
        this->state = this->goto_state;
        this->goto_state = State::IDLE;
    }
};


