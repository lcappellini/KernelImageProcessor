//
// Created by Lorenzo on 30/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_PIXELIMAGE_H
#define KERNELIMAGEPROCESSOR_PIXELIMAGE_H

#include <vector>
#include "Image.h"

using namespace std;

template <uint8_t numChannels>
class PixelImage : public Image {
private:
    uint8_t * pixels;
public:
    PixelImage<numChannels>(unsigned short w, unsigned short h, uint8_t * pixelData);
    uint8_t * get_at(int index) const override;
    uint8_t * get_at(int x, int y) const override;
    void set_at(int index, uint8_t * values) override;
    void set_at(int x, int y, uint8_t * values) override;
};

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
uint8_t *PixelImage<numChannels>::get_at(int index) const {
    auto *pixel = new uint8_t[nChannels]; //TODO use some fancier method to copy the array data
    for (int i = 0; i < nChannels; i++) {
        pixel[i] = pixels[nChannels*index+i];
    }
    return pixel;
}

template <uint8_t numChannels>
uint8_t * PixelImage<numChannels>::get_at(int x, int y) const {
    return get_at(x+y*width);
}


#endif //KERNELIMAGEPROCESSOR_PIXELIMAGE_H
