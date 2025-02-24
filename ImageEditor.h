//
// Created by Lorenzo on 01/02/2025.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGEEDITOR_H
#define KERNELIMAGEPROCESSOR_IMAGEEDITOR_H


#include "Image.h"
#include "Kernel.h"
#include "KernelMode.h"

namespace ImageEditor {
    uint16_t rgbToGrayscale(const uint16_t *pixel);
    Image * CreatePixelImage(int chs, int bitDepth, int resW, int resH, void *pixelData);
    //kernel image processing
    Image * convolve(Image * image, Kernel *kernel, KernelMode kernelMode = KernelMode::Crop, const uint8_t * fillColor = nullptr);
    Image * sharpen(Image *image);
    Image * gaussian_blur3(Image *image);
    //other effects
    Image * invert(Image * image);
    Image * change_brightness(Image *image, float factor);
    Image * grayscale(Image *image);
    Image * crop(Image *image, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    Image * edge_detection_effect(Image *image);
    Image * emboss(Image *image);
}

#endif //KERNELIMAGEPROCESSOR_IMAGEEDITOR_H
