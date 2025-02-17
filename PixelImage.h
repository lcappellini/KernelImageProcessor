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

template <uint8_t numChannels, typename T>
class PixelImage : public Image {
private:
    T * pixels;
    uint16_t * get_at_unsafe(int index) const;
public:
    PixelImage<numChannels, typename T>(uint16_t w, uint16_t h, T * pixelData);
    PixelImage<numChannels, typename T>(uint16_t w, uint16_t h);
    uint16_t * get_at(int index) const override;
    uint16_t * get_at(int x, int y) const override;
    void set_at(int index, uint16_t * values) override;
    void set_at(int x, int y, uint16_t * values) override;
};

template <uint8_t numChannels, typename T>
PixelImage<numChannels, T>::PixelImage(unsigned short w, unsigned short h) {
    width = w;
    height = h;
    nChannels = numChannels;
    pixels = new T[width*height*nChannels]();
    bitDepth = sizeof(T)*8;
}

template <uint8_t numChannels, typename T>
void PixelImage<numChannels, T>::set_at(int x, int y, uint16_t * values) {
    set_at(x+y*width, values);
}

template <uint8_t numChannels, typename T>
void PixelImage<numChannels, T>::set_at(int index, uint16_t * values) {
    for (int i = 0; i < nChannels; i++){
        pixels[index*nChannels+i] = values[i];
    }
}

template <uint8_t numChannels, typename T>
PixelImage<numChannels, T>::PixelImage(unsigned short w, unsigned short h, T * pixelData) {
    width = w;
    height = h;
    pixels = pixelData;
    nChannels = numChannels;
    bitDepth = sizeof(T)*8;
}

template <uint8_t numChannels, typename T>
uint16_t * PixelImage<numChannels, T>::get_at_unsafe(int index) const {
    auto * pixel = new uint16_t[nChannels]; //TODO use some fancier method to copy the array data
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = pixels[nChannels*index+i];
    }
    return pixel;
}

template <uint8_t numChannels, typename T>
uint16_t * PixelImage<numChannels, T>::get_at(int index) const {
    if (index < 0 || index >= width*height*nChannels)
        throw out_of_range("Index out of bounds");
    return get_at_unsafe(index);
}

template <uint8_t numChannels, typename T>
uint16_t * PixelImage<numChannels, T>::get_at(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw out_of_range("Indexes out of bounds");
    }
    return get_at_unsafe(x+y*width);
}


#endif //KERNELIMAGEPROCESSOR_PIXELIMAGE_H

