//
// Created by Lorenzo on 30/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGE_H
#define KERNELIMAGEPROCESSOR_IMAGE_H

#include "Channel.h"

template <uint8_t numChannels> class Image {
private:
    unsigned short width;
    unsigned short height;
    Channel channels[numChannels];
public:
    Image(unsigned short w, unsigned short h, Channel * channels);
    unsigned short get_width();
    unsigned short get_height();
};


#endif //KERNELIMAGEPROCESSOR_IMAGE_H
