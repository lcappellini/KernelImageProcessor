//
// Created by Lorenzo on 30/01/2025.
//

#include "Image.h"


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
}