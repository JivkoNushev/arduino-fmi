#pragma once

#include <stdint.h>

#include "config.h"
#include "queue.h"

class ElevatorButton {
public:
    ElevatorButton(int pin, int floor, bool going_up) : 
        pin(pin),
        floor(floor),
        going_up(going_up)
    {}

    void begin() {
        pinMode(this->pin, INPUT_PULLUP);
    }

    void update() {
        if(this->pin == 0) return;

        bool reading = digitalRead(this->pin);

        if (reading != this->last_reading) {
            this->last_debounce_time = millis();
        }

        if (millis() - this->last_debounce_time > 0) {
            if (reading != this->stable_state) {
                this->stable_state = reading;

                if (this->stable_state == LOW) {
                    this->clicked = true;
                }
            }
        }

        this->last_reading = reading;
    }

    void handle_if_clicked(StaticQueue<int>& queue) {
        if(this->clicked) {
            Serial.print("Clicked button: ");
            Serial.println(this->pin);
            queue.push_unique(this->floor);
            this->clicked = false;
        }
    }

private:
    uint8_t pin;
    uint8_t floor;
    bool going_up;
    bool clicked;

    bool stable_state;
    bool last_reading;
    uint32_t last_debounce_time;
    uint32_t last_release_time;
};

class ElevatorButtons {
public:
    ElevatorButtons() :
        b0(BUTTON0_PIN, 1, true),
        b1(BUTTON1_PIN, 2, false),
        b2(BUTTON2_PIN, 2, true),
        b3(BUTTON3_PIN, 3, false),
        b4(BUTTON4_PIN, 1, false),
        b5(BUTTON5_PIN, 2, false),
        b6(BUTTON6_PIN, 3, false)
    {}

    void begin() {
        this->b0.begin();
        this->b1.begin();
        this->b2.begin();
        this->b3.begin();
        this->b4.begin();
        this->b5.begin();
        this->b6.begin();
    }

    void update() {
        this->b0.update();
        this->b0.handle_if_clicked(this->queue);

        this->b1.update();
        this->b1.handle_if_clicked(this->queue);

        this->b2.update();
        this->b2.handle_if_clicked(this->queue);

        this->b3.update();
        this->b3.handle_if_clicked(this->queue);

        this->b4.update();
        this->b4.handle_if_clicked(this->queue);

        this->b5.update();
        this->b5.handle_if_clicked(this->queue);

        this->b6.update();
        this->b6.handle_if_clicked(this->queue);
    }

    int next_floor() {
        if (this->queue.empty()) return -1;

        int floor = this->queue.peek();
        this->queue.pop();

        Serial.print("Next floor: ");
        Serial.println(floor);

        return floor;
    }

private:
    ElevatorButton b0;
    ElevatorButton b1;
    ElevatorButton b2;
    ElevatorButton b3;
    ElevatorButton b4;
    ElevatorButton b5;
    ElevatorButton b6;

    StaticQueue<int> queue;
};


