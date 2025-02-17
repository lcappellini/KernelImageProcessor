//
// Created by Lorenzo on 30/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_PIXELIMAGE_H
#define KERNELIMAGEPROCESSOR_PIXELIMAGE_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include "Image.h"

using namespace std;

template<uint8_t bits>
using pixel_type = typename std::conditional<bits == 8, uint8_t, uint16_t>::type;

template <uint8_t numChannels, uint8_t bits>
class PixelImage : public Image {
private:
    pixel_type<bits> * pixels;
    uint16_t * get_at_unsafe(int index) const;
public:
    PixelImage<numChannels, bits>(uint16_t w, uint16_t h, pixel_type<bits> * pixelData);
    PixelImage<numChannels, bits>(uint16_t w, uint16_t h);
    uint16_t * get_at(int index) const override;
    uint16_t * get_at(int x, int y) const override;
    void set_at(int index, uint16_t * values) override;
    void set_at(int x, int y, uint16_t * values) override;
};

template <uint8_t numChannels, uint8_t bits>
PixelImage<numChannels, bits>::PixelImage(unsigned short w, unsigned short h) {
    width = w;
    height = h;
    nChannels = numChannels;
    pixels = new pixel_type<bits>[width*height*nChannels]();
    bitDepth = bits;
}

template <uint8_t numChannels, uint8_t bits>
void PixelImage<numChannels, bits>::set_at(int x, int y, uint16_t * values) {
    set_at(x+y*width, values);
}

template <uint8_t numChannels, uint8_t bits>
void PixelImage<numChannels, bits>::set_at(int index, uint16_t * values) {
    for (int i = 0; i < nChannels; i++){
        pixels[index*nChannels+i] = values[i];
    }
}

template <uint8_t numChannels, uint8_t bits>
PixelImage<numChannels, bits>::PixelImage(unsigned short w, unsigned short h, pixel_type<bits> * pixelData) {
    width = w;
    height = h;
    pixels = pixelData;
    nChannels = numChannels;
    bitDepth = bits;
}

template <uint8_t numChannels, uint8_t bits>
uint16_t * PixelImage<numChannels, bits>::get_at_unsafe(int index) const {
    auto * pixel = new uint16_t[nChannels];
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = pixels[nChannels*index+i];
    }
    return pixel;
}

template <uint8_t numChannels, uint8_t bits>
uint16_t * PixelImage<numChannels, bits>::get_at(int index) const {
    if (index < 0 || index >= width*height*nChannels)
        throw out_of_range("Index out of bounds");
    return get_at_unsafe(index);
}

template <uint8_t numChannels, uint8_t bits>
uint16_t * PixelImage<numChannels, bits>::get_at(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw out_of_range("Indexes out of bounds");
    }
    return get_at_unsafe(x+y*width);
}


#endif //KERNELIMAGEPROCESSOR_PIXELIMAGE_H

