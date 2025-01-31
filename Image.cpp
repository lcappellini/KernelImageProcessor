//
// Created by Lorenzo on 30/01/2025.
//

#include "Image.h"


/*
template<uint8_t numChannels>
unsigned short Image<numChannels>::get_width() {
    return width;
}

template<uint8_t numChannels>
unsigned short Image<numChannels>::get_height() {
    return height;
}

template<uint8_t numChannels>
Image<numChannels>::Image(unsigned short w, unsigned short h, Channel * chs) {
    width = w;
    height = h;
    channels = chs;
}*/


unsigned short Image::get_width() {
    return width;
}

unsigned short Image::get_height() {
    return height;
}

Image::Image(unsigned short w, unsigned short h, uint8_t nChs, Channel * chs) {
    width = w;
    height = h;
    nChannels = nChs;
    channels = chs;
}

uint8_t *Image::get_at(int index) {
    auto *pixel = new uint8_t[nChannels];
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = channels[i].get_at(index);
    }
    return pixel;
}

uint8_t *Image::get_at(int x, int y) {
    return get_at(x+y*width);
}