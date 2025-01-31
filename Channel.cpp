//
// Created by Lorenzo on 30/01/2025.
//

#include "Channel.h"

uint8_t Channel::get_at(int index) {
    return buffer[index];
}

uint8_t Channel::get_at(int x, int y) {
    return buffer[y*height+x];
}

Channel::Channel(unsigned short w, unsigned short h) {
    width = w;
    height = h;
    buffer = new uint8_t[width*height];
}

Channel::Channel(unsigned short w, unsigned short h, uint8_t* buff) {
    width = w;
    height = h;
    buffer = buff;
}

void Channel::set_at(int index, uint8_t value) {
    buffer[index] = value;
}

void Channel::set_at(int x, int y, uint8_t value) {
    set_at(y*height+x, value);
}



