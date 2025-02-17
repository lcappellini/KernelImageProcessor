//
// Created by Lorenzo on 31/01/2025.
//

#include "Image.h"


uint16_t Image::get_width() const {
    return width;
}

uint16_t Image::get_height() const {
    return height;
}

uint8_t Image::get_nChannels() const {
    return nChannels;
}

uint8_t Image::get_bitDepth() const {
    return bitDepth;
}
