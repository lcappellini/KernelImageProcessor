//
// Created by Lorenzo on 01/02/2025.
//

#include <iostream>
#include "ImageEditor.h"
#include "KernelMode.h"
#include "PixelImage.h"

Image *ImageEditor::convolve(Image *image, Kernel *kernel, KernelMode kernelMode) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();
    //TODO should i enable raw pixel data access to speed up the process?
    //TODO asserting kernel size is ODD, check if code works with EVEN kernel size
    uint16_t size = kernel->get_size();
    uint16_t halfSize = size / 2;
    uint16_t resW, resH;
    if (kernelMode == KernelMode::Crop) {
        resW = w-2*halfSize;
        resH = h-2*halfSize;
    } else {
        resW = w;
        resH = h;
    }

    Image * resImage;
    auto * pixelData = new uint8_t[resW*resH*chs];
    if (chs == 1){
        resImage = new PixelImage<1>(resW, resH, pixelData);
    } else if (chs == 2){
        resImage = new PixelImage<2>(resW, resH, pixelData);
    } else if (chs == 3){
        resImage = new PixelImage<3>(resW, resH, pixelData);
    } else if (chs == 4){
        resImage = new PixelImage<4>(resW, resH, pixelData);
    }

    int x0, x1, y0, y1;
    if (kernelMode == KernelMode::Crop){
        x0 = halfSize;
        x1 = w-halfSize;
        y0 = halfSize;
        y1 = h-halfSize;
    } else {
        x0 = 0;
        x1 = w;
        y0 = 0;
        y1 = h;
    }

    if (kernelMode == KernelMode::Crop){
        int n = 0;
        for (int y = y0; y < y1; y++){
            for (int x = x0; x < x1; x++){
                for (int c = 0; c < chs; c++) {
                    float sum = 0;
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            sum += (float)image->get_at(x-halfSize+j, y-halfSize+i)[c] * kernel->get_at(j, i);
                        }
                    }
                    if (sum > 255)
                        sum = 255;
                    else if (sum < 0)
                        sum = 0;
                    pixelData[n] = (uint8_t)sum;
                    n++;
                }
            }
        }
    } else if (kernelMode == KernelMode::Extend){
        int n = 0;
        int px, py;
        for (int y = y0; y < y1; y++){
            for (int x = x0; x < x1; x++){
                for (int c = 0; c < chs; c++) {
                    float sum = 0;
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            if (x-halfSize+j < 0){
                                px = 0;
                            } else if (x-halfSize+j >= w){
                                px = w-1;
                            } else {
                                px = x-halfSize+j;
                            }
                            if (y-halfSize+i < 0){
                                py = 0;
                            } else if (y-halfSize+i >= h){
                                py = h-1;
                            } else {
                                py = y-halfSize+i;
                            }
                            sum += (float)image->get_at(px, py)[c] * kernel->get_at(j, i);
                        }
                    }
                    if (sum > 255)
                        sum = 255;
                    else if (sum < 0)
                        sum = 0;
                    pixelData[n] = (uint8_t)sum;
                    n++;
                }
            }
        }
    }

    return resImage;
}
