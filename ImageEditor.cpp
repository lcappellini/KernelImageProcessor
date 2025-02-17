//
// Created by Lorenzo on 01/02/2025.
//

#include <iostream>
#include "ImageEditor.h"
#include "KernelMode.h"
#include "PixelImage.h"

using namespace std;


Image *ImageEditor::convolve(Image *image, Kernel *kernel, KernelMode kernelMode, const uint8_t * fillColor) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();
    //TODO should i enable raw pixel data access to speed up the process?
    //TODO assuming kernel size is ODD, check if code works with EVEN kernel size
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

    uint8_t bitDepth = image->get_bitDepth();
    float maxBitValue = (float)pow(2, bitDepth)-1;

    Image * resImage;
    void * pixelData;
    if (bitDepth == 8) {
        pixelData = new uint8_t[resW * resH * chs];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[resW * resH * chs];
    }

    if (chs == 1) {
        if (bitDepth == 8)
            resImage = new PixelImage<1, uint8_t>(resW, resH, (uint8_t *)pixelData);
        else
            resImage = new PixelImage<1, uint16_t>(resW, resH, (uint16_t *)pixelData);
    } else if (chs == 2) {
        if (bitDepth == 8)
            resImage = new PixelImage<2, uint8_t>(resW, resH, (uint8_t *)pixelData);
        else
            resImage = new PixelImage<2, uint16_t>(resW, resH, (uint16_t *)pixelData);
    } else if (chs == 3) {
        if (bitDepth == 8)
            resImage = new PixelImage<3, uint8_t>(resW, resH, (uint8_t *)pixelData);
        else
            resImage = new PixelImage<3, uint16_t>(resW, resH, (uint16_t *)pixelData);
    } else if (chs == 4) {
        if (bitDepth == 8)
            resImage = new PixelImage<4, uint8_t>(resW, resH, (uint8_t *)pixelData);
        else
            resImage = new PixelImage<4, uint16_t>(resW, resH, (uint16_t *)pixelData);
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
                    if (sum > maxBitValue)
                        sum = maxBitValue;
                    else if (sum < 0)
                        sum = 0;
                    if (bitDepth == 8)
                        reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)sum;
                    else
                        reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)sum;
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
                    if (sum > maxBitValue)
                        sum = maxBitValue;
                    else if (sum < 0)
                        sum = 0;
                    if (bitDepth == 8)
                        reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)sum;
                    else
                        reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)sum;
                    n++;
                }
            }
        }
    } else if (kernelMode == KernelMode::Wrap){
        int n = 0;
        int px, py;
        for (int y = y0; y < y1; y++){
            for (int x = x0; x < x1; x++){
                for (int c = 0; c < chs; c++) {
                    float sum = 0;
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            if (x-halfSize+j < 0){
                                px = x-halfSize+j+w;
                            } else if (x-halfSize+j >= w){
                                px = x-halfSize+j-w;
                            } else {
                                px = x-halfSize+j;
                            }
                            if (y-halfSize+i < 0){
                                py = y-halfSize+i+h;
                            } else if (y-halfSize+i >= h){
                                py = y-halfSize+i-h;
                            } else {
                                py = y-halfSize+i;
                            }
                            sum += (float)image->get_at(px, py)[c] * kernel->get_at(j, i);
                        }
                    }
                    if (sum > maxBitValue)
                        sum = maxBitValue;
                    else if (sum < 0)
                        sum = 0;
                    if (bitDepth == 8)
                        reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)sum;
                    else
                        reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)sum;
                    n++;
                }
            }
        }
    } else if (kernelMode == KernelMode::Mirror){
        int n = 0;
        int px, py;
        for (int y = y0; y < y1; y++){
            for (int x = x0; x < x1; x++){
                for (int c = 0; c < chs; c++) {
                    float sum = 0;
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            if (x-halfSize+j < 0){
                                px = -(x-halfSize+j);
                            } else if (x-halfSize+j >= w){
                                px = w-(x-halfSize+j-w)-1;
                            } else {
                                px = x-halfSize+j;
                            }
                            if (y-halfSize+i < 0){
                                py = -(y-halfSize+i);
                            } else if (y-halfSize+i >= h){
                                py = h-(y-halfSize+i-h)-1;
                            } else {
                                py = y-halfSize+i;
                            }
                            sum += (float)image->get_at(px, py)[c] * kernel->get_at(j, i);
                        }
                    }
                    if (sum > maxBitValue)
                        sum = maxBitValue;
                    else if (sum < 0)
                        sum = 0;
                    if (bitDepth == 8)
                        reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)sum;
                    else
                        reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)sum;
                    n++;
                }
            }
        }
    } else if (kernelMode == KernelMode::KernelCrop){
        int n = 0;
        int px, py;
        for (int y = y0; y < y1; y++){
            for (int x = x0; x < x1; x++){
                for (int c = 0; c < chs; c++) {
                    float sum = 0;
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            if (x-halfSize+j < 0){
                                continue;
                            } else if (x-halfSize+j >= w){
                                continue;
                            } else {
                                px = x-halfSize+j;
                            }
                            if (y-halfSize+i < 0){
                                continue;
                            } else if (y-halfSize+i >= h){
                                continue;
                            } else {
                                py = y-halfSize+i;
                            }
                            sum += (float)image->get_at(px, py)[c] * kernel->get_at(j, i);
                        }
                    }
                    if (sum > maxBitValue)
                        sum = maxBitValue;
                    else if (sum < 0)
                        sum = 0;
                    if (bitDepth == 8)
                        reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)sum;
                    else
                        reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)sum;
                    n++;
                }
            }
        }
    } else if (kernelMode == KernelMode::Constant){
        if (fillColor == nullptr){
            throw invalid_argument("The parameter fillColor is required with KernelMode::Constant");
        }
        int n = 0;
        int px, py;
        for (int y = y0; y < y1; y++){
            for (int x = x0; x < x1; x++){
                for (int c = 0; c < chs; c++) {
                    float sum = 0;
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            bool outside = false;
                            if (x-halfSize+j < 0){
                                outside = true;
                            } else if (x-halfSize+j >= w){
                                outside = true;
                            } else {
                                px = x-halfSize+j;
                            }
                            if (y-halfSize+i < 0){
                                outside = true;
                            } else if (y-halfSize+i >= h){
                                outside = true;
                            } else {
                                py = y-halfSize+i;
                            }
                            if (outside)
                                sum += (float)fillColor[c] * kernel->get_at(j, i);
                            else
                                sum += (float)image->get_at(px, py)[c] * kernel->get_at(j, i);
                        }
                    }
                    if (sum > maxBitValue)
                        sum = maxBitValue;
                    else if (sum < 0)
                        sum = 0;
                    if (bitDepth == 8)
                        reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)sum;
                    else
                        reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)sum;
                    n++;
                }
            }
        }
    }
    return resImage;
}

Image * ImageEditor::sharpen(Image * image){
    Kernel * ker = new Kernel(3, new float[9]{0, -1, 0, -1, 5, -1, 0, -1, 0});
    Image * resImage = ImageEditor::convolve(image, ker, KernelMode::Constant, new uint8_t[]{0, 0, 0}); //Constant with (0,0,0) should be the best for sharpening
    return resImage;
}

Image * ImageEditor::gaussian_blur3(Image * image){
    Kernel * ker = new Kernel(3, new float[9]{1/16.0, 1/8.0, 1/16.0, 1/8.0, 1/4.0, 1/8.0, 1/16.0, 1/8.0, 1/16.0});
    Image * resImage = ImageEditor::convolve(image, ker, KernelMode::Mirror); //Mirror should be the best for gaussian blur
    return resImage;
}

Image * ImageEditor::invert(Image *image) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();
    uint8_t bitDepth = image->get_bitDepth();

    void * pixelData;
    if (bitDepth == 8) {
        pixelData = new uint8_t[w * h * chs];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[w * h * chs];
    }

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint16_t * pixel = image->get_at(i);
        for (int c = 0; c < chs; c++) {
            if (bitDepth == 8)
                reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)255-pixel[c];
            else
                reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)65536-pixel[c];
            n++;
        }
    }

    return CreatePixelImage(chs, bitDepth, w, h, pixelData);
}

Image * ImageEditor::change_brightness(Image *image, float factor) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();
    uint8_t bitDepth = image->get_bitDepth();
    uint16_t maxBitValue = (uint32_t)pow(2, bitDepth)-1;

    int32_t value = (int32_t)((factor - 1)*(float)maxBitValue);

    void * pixelData;
    if (bitDepth == 8) {
        pixelData = new uint8_t[w * h * chs];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[w * h * chs];
    }

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint16_t * pixel = image->get_at(i);
        for (int c = 0; c < chs; c++) {
            int result = value+pixel[c];
            if (result > maxBitValue)
                result = maxBitValue;
            else if (result < 0)
                result = 0;
            if (bitDepth == 8)
                reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)result;
            else
                reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)result;
            n++;
        }
    }

    return CreatePixelImage(chs, bitDepth, w, h, pixelData);
}

uint16_t ImageEditor::rgbToGrayscale(const uint16_t * pixel) {
    return (uint16_t)(0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2]);
}

Image * ImageEditor::grayscale(Image *image) {
    uint8_t chs = image->get_nChannels();
    if (chs < 3){
        throw std::runtime_error("Image is already grayscale");
    }

    uint16_t w = image->get_width();
    uint16_t h = image->get_height();
    uint8_t bitDepth = image->get_bitDepth();

    /*void * pixelData;
    if (bitDepth == 8) {
        pixelData = new uint8_t[w * h * chs];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[w * h * chs];
    }

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint16_t * pixel = image->get_at(i);
        uint16_t grayscale = rgbToGrayscale(pixel);
        for (int c = 0; c < chs; c++) {
            if (bitDepth == 8)
                reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)grayscale;
            else
                reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)grayscale;
            n++;
        }
    }*/

    void * pixelData;
    if (bitDepth == 8) {
        pixelData = new uint8_t[w * h * 1];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[w * h * 1];
    }

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint16_t * pixel = image->get_at(i);
        uint16_t grayscale = rgbToGrayscale(pixel);
        if (bitDepth == 8)
            reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)grayscale;
        else
            reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)grayscale;
        n++;
    }

    return CreatePixelImage(1, bitDepth, w, h, pixelData);
}

Image *ImageEditor::crop(Image *image, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();
    uint8_t bitDepth = image->get_bitDepth();

    if (x0 >= x1 || y0 >= y1 || x1 >= w || y1 >= h || x0 < 0 || y0 < 0) {
        throw invalid_argument("Parameters x0,y0,x1,y1 must satisfy 0<=x0<x1<width and 0<=y0<y1<height");
    }

    void * pixelData;
    if (bitDepth == 8) {
        pixelData = new uint8_t[(x1-x0) * (y1-y0) * chs];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[(x1-x0) * (y1-y0) * chs];
    }

    int n = 0;
    for (int y = y0; y < y1; y++){
        for (int x = x0; x < x1; x++) {
            uint16_t * pixel = image->get_at(x, y);
            for (int c = 0; c < chs; c++) {
                if (bitDepth == 8)
                    reinterpret_cast<uint8_t *>(pixelData)[n] = (uint8_t)pixel[c];
                else
                    reinterpret_cast<uint16_t *>(pixelData)[n] = (uint16_t)pixel[c];
                n++;
            }
        }
    }

    return CreatePixelImage(chs, bitDepth, x1-x0, y1-y0, pixelData);
}


Image * ImageEditor::CreatePixelImage(int chs, int bitDepth, int resW, int resH, void* pixelData) {
    if (bitDepth == 8) {
        switch (chs) {
            case 1: return new PixelImage<1, uint8_t>(resW, resH, (uint8_t*)pixelData);
            case 2: return new PixelImage<2, uint8_t>(resW, resH, (uint8_t*)pixelData);
            case 3: return new PixelImage<3, uint8_t>(resW, resH, (uint8_t*)pixelData);
            case 4: return new PixelImage<4, uint8_t>(resW, resH, (uint8_t*)pixelData);
        }
    } else {
        switch (chs) {
            case 1: return new PixelImage<1, uint16_t>(resW, resH, (uint16_t*)pixelData);
            case 2: return new PixelImage<2, uint16_t>(resW, resH, (uint16_t*)pixelData);
            case 3: return new PixelImage<3, uint16_t>(resW, resH, (uint16_t*)pixelData);
            case 4: return new PixelImage<4, uint16_t>(resW, resH, (uint16_t*)pixelData);
        }
    }
    return nullptr;
}












