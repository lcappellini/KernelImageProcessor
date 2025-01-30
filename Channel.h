//
// Created by Lorenzo on 30/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_CHANNEL_H
#define KERNELIMAGEPROCESSOR_CHANNEL_H

#include <cstdint>

class Channel {
private:
    uint8_t * buffer;
    int width;
    int height;
public:
    Channel(unsigned short w, unsigned short h);
    Channel(unsigned short w, unsigned short h, uint8_t* buff);
    uint8_t get_at(int index);
    uint8_t get_at(int x, int y);
    void set_at(int index, uint8_t value);
    void set_at(int x, int y, uint8_t value);
};


#endif //KERNELIMAGEPROCESSOR_CHANNEL_H
