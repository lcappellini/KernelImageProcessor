//
// Created by Lorenzo on 31/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGE_H
#define KERNELIMAGEPROCESSOR_IMAGE_H


#include <cstdint>

class Image {
protected:
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t nChannels = 0;
public:
    uint16_t get_width() const;
    uint16_t get_height() const;
    virtual uint8_t * get_at(int index) const = 0;
    virtual uint8_t * get_at(int x, int y) const = 0;
    virtual void set_at(int index, uint8_t * values) = 0;
    virtual void set_at(int x, int y, uint8_t * values) = 0;
    uint8_t get_nChannels() const;
};


#endif //KERNELIMAGEPROCESSOR_IMAGE_H
