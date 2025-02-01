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

template <uint8_t numChannels>
class PixelImage : public Image {
private:
    uint8_t * pixels;
    uint8_t * get_at_unsafe(int index) const;
public:
    PixelImage<numChannels>(uint16_t w, uint16_t h, uint8_t * pixelData);
    PixelImage<numChannels>(uint16_t w, uint16_t h);
    uint8_t * get_at(int index) const override;
    uint8_t * get_at(int x, int y) const override;
    void set_at(int index, uint8_t * values) override;
    void set_at(int x, int y, uint8_t * values) override;

};

template<uint8_t numChannels>
PixelImage<numChannels>::PixelImage(unsigned short w, unsigned short h) {
    width = w;
    height = h;
    nChannels = numChannels;
    pixels = new uint8_t[width*height*nChannels]();
}

template<uint8_t numChannels>
void PixelImage<numChannels>::set_at(int x, int y, uint8_t * values) {
    set_at(x+y*width, values);
}

template<uint8_t numChannels>
void PixelImage<numChannels>::set_at(int index, uint8_t * values) {
    for (int i = 0; i < nChannels; i++){
        pixels[index*nChannels+i] = values[i];
    }
}

template <uint8_t numChannels>
PixelImage<numChannels>::PixelImage(unsigned short w, unsigned short h, uint8_t * pixelData) {
    width = w;
    height = h;
    pixels = pixelData;
    nChannels = numChannels;
}

template <uint8_t numChannels>
uint8_t *PixelImage<numChannels>::get_at_unsafe(int index) const {
    auto *pixel = new uint8_t[nChannels]; //TODO use some fancier method to copy the array data
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = pixels[nChannels*index+i];
    }
    return pixel;
}

template <uint8_t numChannels>
uint8_t *PixelImage<numChannels>::get_at(int index) const {
    if (index < 0 || index >= width*height*nChannels)
        throw out_of_range("Index out of bounds");
    return get_at_unsafe(index);
}

template <uint8_t numChannels>
uint8_t * PixelImage<numChannels>::get_at(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw out_of_range("Indexes out of bounds");
    }
    return get_at_unsafe(x+y*width);
}


#endif //KERNELIMAGEPROCESSOR_PIXELIMAGE_H
