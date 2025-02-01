//
// Created by Lorenzo on 01/02/2025.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGEEDITOR_H
#define KERNELIMAGEPROCESSOR_IMAGEEDITOR_H


#include "Image.h"
#include "Kernel.h"
#include "KernelMode.h"

class ImageEditor {
public:
    static Image * convolve(Image *image, Kernel *kernel, KernelMode kernelMode = KernelMode::Crop);
    //TODO add more "effects" like: crop, scale, ecc
};


#endif //KERNELIMAGEPROCESSOR_IMAGEEDITOR_H
