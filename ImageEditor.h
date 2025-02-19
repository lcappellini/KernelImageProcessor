//
// Created by Lorenzo on 01/02/2025.
//

#ifndef KERNELIMAGEPROCESSOR_IMAGEEDITOR_H
#define KERNELIMAGEPROCESSOR_IMAGEEDITOR_H


#include "Image.h"
#include "Kernel.h"
#include "KernelMode.h"

class ImageEditor {
private:
    static uint16_t rgbToGrayscale(const uint16_t *pixel);
    static Image * CreatePixelImage(int chs, int bitDepth, int resW, int resH, void *pixelData);
public:
    //kernel image processing
    static Image * convolve(Image * image, Kernel *kernel, KernelMode kernelMode = KernelMode::Crop, const uint8_t * fillColor = nullptr);
    static Image * sharpen(Image *image);
    static Image * gaussian_blur3(Image *image);
    //other effects
    static Image * invert(Image * image);
    static Image * change_brightness(Image *image, float factor);
    static Image * grayscale(Image *image);
    static Image * crop(Image *image, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    static Image *edge_detection_effect(Image *image);

    static Image *emboss(Image *image);
};


#endif //KERNELIMAGEPROCESSOR_IMAGEEDITOR_H
