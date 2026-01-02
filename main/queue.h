#pragma once

#include <stdint.h>

template<class T>
class StaticQueue {
private:
    static const constexpr uint32_t MAX_SIZE = 256;

public:
    bool empty() const {
        return !this->size;
    }

    bool exists(const T& e) const {
        if(this->empty()) return false;

        uint32_t it = this->head;
        for(uint32_t it = this->head; it != this->tail; it = (it + 1) % MAX_SIZE) {
            if(this->data[it] == e) return true;
        }

        return false;
    }

    bool push_unique(const T& e) {
        if(this->exists(e)) return false;

        this->push(e);

        return true;
    }

    bool push(const T& e) {
        if (this->size >= MAX_SIZE) return false;

        this->tail = (this->tail + 1) % MAX_SIZE;

        this->data[this->tail] = e;
        this->size++;

        return true;
    }

    bool pop() {
        if (this->empty()) return false;

        if(this->head == this->tail) {
            this->head = this->tail = 0;
        } else {
            this->head = (this->head + 1) % MAX_SIZE;
        }

        return true;
    }

    const T& peek() const {
        return this->data[this->head];
    }

    T& peek() {
        return this->data[this->head];
    }

private:
    T data[MAX_SIZE];
    uint32_t size = 0;

    uint32_t head = 0;
    uint32_t tail = 0;
};
