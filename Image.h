//
// Created by Lorenzo on 30/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGE_H
#define KERNELIMAGEPROCESSOR_IMAGE_H

#include "Channel.h"

/*template <uint8_t numChannels> class Image {
private:
    unsigned short width;
    unsigned short height;
    Channel channels[numChannels];
public:
    Image(unsigned short w, unsigned short h, Channel * channels);
    unsigned short get_width();
    unsigned short get_height();
};*/

class Image {
private:
    unsigned short width;
    unsigned short height;
    uint8_t nChannels;
    Channel * channels;
public:
    Image(unsigned short w, unsigned short h, uint8_t nChs, Channel * chs);
    unsigned short get_width();
    unsigned short get_height();
    uint8_t * get_at(int index);
    uint8_t * get_at(int x, int y);
};


#endif //KERNELIMAGEPROCESSOR_IMAGE_H
