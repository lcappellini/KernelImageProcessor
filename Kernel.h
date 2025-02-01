//
// Created by Lorenzo on 01/02/2025.
//

#ifndef KERNELIMAGEPROCESSOR_KERNEL_H
#define KERNELIMAGEPROCESSOR_KERNEL_H


#include <cstdint>

class Kernel {
private:
    uint16_t size;
    float * matrix;
    float get_at_unsafe(int index);
public:
    Kernel(uint16_t size, float * matrix);
    uint16_t get_size() const;
    float get_at(int index);
    float get_at(int x, int y);
};


#endif //KERNELIMAGEPROCESSOR_KERNEL_H
