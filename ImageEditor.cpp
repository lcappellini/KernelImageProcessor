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
                    if (sum > 255)
                        sum = 255;
                    else if (sum < 0)
                        sum = 0;
                    pixelData[n] = (uint8_t)sum;
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
                    if (sum > 255)
                        sum = 255;
                    else if (sum < 0)
                        sum = 0;
                    pixelData[n] = (uint8_t)sum;
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
                    if (sum > 255)
                        sum = 255;
                    else if (sum < 0)
                        sum = 0;
                    pixelData[n] = (uint8_t)sum;
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

Image * ImageEditor::invert(Image *image) { //FIXME shouldn't affect alpha value
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();

    uint8_t * pixelData = new uint8_t[w*h*chs];

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint8_t * pixel = image->get_at(i);
        for (int c = 0; c < chs; c++) {
            pixelData[n] = 255-pixel[c];
            n++;
        }
    }

    if (chs == 1) {
        return new PixelImage<1>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 2) {
        return new PixelImage<2>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 3) {
        return new PixelImage<3>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 4) {
        return new PixelImage<4>(image->get_width(), image->get_height(), pixelData);
    }
    return nullptr;
}

Image * ImageEditor::change_brightness(Image *image, int16_t value) { //FIXME shouldn't affect alpha value
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();

    uint8_t * pixelData = new uint8_t[w*h*chs];

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint8_t * pixel = image->get_at(i);
        for (int c = 0; c < chs; c++) {
            int result = value+pixel[c];
            if (result > 255)
                result = 255;
            else if (result < 0)
                result = 0;
            pixelData[n] = result;
            n++;
        }
    }

    if (chs == 1) {
        return new PixelImage<1>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 2) {
        return new PixelImage<2>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 3) {
        return new PixelImage<3>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 4) {
        return new PixelImage<4>(image->get_width(), image->get_height(), pixelData);
    }
    return nullptr;
}

uint8_t ImageEditor::rgbToGrayscale(const uint8_t * pixel) {
    return (uint8_t)(0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2]);
}

Image * ImageEditor::grayscale(Image *image) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();

    uint8_t * pixelData = new uint8_t[w*h*chs];

    int n = 0;
    for (int i = 0; i < w*h; i++){
        uint8_t * pixel = image->get_at(i);
        uint8_t grayscale = rgbToGrayscale(pixel);
        for (int c = 0; c < chs; c++) {
            pixelData[n] = grayscale;
            n++;
        }
    }

    if (chs == 1) {
        return new PixelImage<1>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 2) {
        return new PixelImage<2>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 3) {
        return new PixelImage<3>(image->get_width(), image->get_height(), pixelData);
    } else if (chs == 4) {
        return new PixelImage<4>(image->get_width(), image->get_height(), pixelData);
    }
    return nullptr;
}

Image *ImageEditor::crop(Image *image, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t chs = image->get_nChannels();
    uint16_t w = image->get_width();
    uint16_t h = image->get_height();

    if (x0 >= x1 || y0 >= y1 || x1 >= w || y1 >= h || x0 < 0 || y0 < 0) {
        throw invalid_argument("Parameters x0,y0,x1,y1 must satisfy 0<=x0<x1<width and 0<=y0<y1<height");
    }

    uint8_t * pixelData = new uint8_t[(x1-x0)*(y1-y0)*chs];

    int n = 0;
    for (int y = y0; y < y1; y++){
        for (int x = x0; x < x1; x++) {
            uint8_t * pixel = image->get_at(x, y);
            for (int c = 0; c < chs; c++) {
                pixelData[n] = pixel[c];
                n++;
            }
        }
    }

    if (chs == 1) {
        return new PixelImage<1>(x1-x0, y1-y0, pixelData);
    } else if (chs == 2) {
        return new PixelImage<2>(x1-x0, y1-y0, pixelData);
    } else if (chs == 3) {
        return new PixelImage<3>(x1-x0, y1-y0, pixelData);
    } else if (chs == 4) {
        return new PixelImage<4>(x1-x0, y1-y0, pixelData);
    }
    return nullptr;
}















